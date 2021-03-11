package trueblocks

/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/

import (
	"fmt"
	"log"
	"net"
	"net/http"

	"github.com/gorilla/websocket"
)

// MessageType is a message carried in a string
type MessageType string

const (
	// CommandErrorMessage is a message send when the server encounters an error
	CommandErrorMessage  MessageType = "command_error"
	// CommandOutputMessage is currently not used, but may in the future carry the actual data
	CommandOutputMessage MessageType = "output"
	// ProgressMessage is a message carried on the stderr stream
	ProgressMessage      MessageType = "progress"
)

var upgrader = websocket.Upgrader{}

// Message is a structure used to send messages via websockets
type Message struct {
	Action   MessageType      `json:"action"`
	ID       string           `json:"id"`
	Content  string           `json:"content"`
}

// Connection is a structure representing a websocket connection
type Connection struct {
	connection *websocket.Conn
	pool       *ConnectionPool
	send       chan *Message
}

// write the message to the connection
func (c *Connection) write() {
	defer func() {
		c.connection.Close()
	}()

	for {
		select {
		case message, ok := <-c.send:
			if !ok {
				c.Log("Connection closed")
				c.connection.WriteMessage(websocket.CloseMessage, []byte{})
				return
			}

			err := c.connection.WriteJSON(message)
			if err != nil {
				// c.Log("Error while sending message, dropping connection: %s", err.Error())
				c.pool.unregister <- c
			}
		}
	}
}

// RemoteAddr is the other end of the connection
func (c *Connection) RemoteAddr() net.Addr {
	return c.connection.RemoteAddr()
}

// Log writes a log messages to the server's stderr
func (c *Connection) Log(s string, args ...interface{}) {
	log.Printf("%s %s\n", c.RemoteAddr(), fmt.Sprintf(s, args...))
}

// ConnectionPool is the collection of all connections
type ConnectionPool struct {
	connections map[*Connection]bool
	broadcast   chan *Message
	register    chan *Connection
	unregister  chan *Connection
}

// closeAndDelete cleans up a connection
func closeAndDelete(pool *ConnectionPool, connection *Connection) {
	delete(pool.connections, connection)
	close(connection.send)
}

// newConnectionPool returns a new connection structure
func newConnectionPool() *ConnectionPool {
	return &ConnectionPool{
		connections: make(map[*Connection]bool),
		broadcast:   make(chan *Message),
		register:    make(chan *Connection),
		unregister:  make(chan *Connection),
	}
}

// run the web sockets server
func (pool *ConnectionPool) run() {
	// run forever...
	for {
		select {
		// handle a signal to register a new connection
		case connection := <-pool.register:
			connection.Log("Connected (Websockets)")
			pool.connections[connection] = true
		// handle a signal to unregister a connection
		case connection := <-pool.unregister:
			if _, ok := pool.connections[connection]; ok {
				connection.Log("Unregistering connection")
				closeAndDelete(pool, connection)
			}
		// handle a signal to broadcast a message
		case message := <-pool.broadcast:
			for connection := range pool.connections {
				connection.send <- message
			}
		}
	}
}

// HandleWebsockets handles web sockets
func HandleWebsockets(pool *ConnectionPool, w http.ResponseWriter, r *http.Request) {
	// TODO: the line below allows any connection through WebSockets. Once the server
	// TODO: is ready, we should implement some rules here
	upgrader.CheckOrigin = func(r *http.Request) bool { return true }

	c, err := upgrader.Upgrade(w, r, nil)
	if err != nil {
		log.Print("upgrade:", err)
		return
	}

	connection := &Connection{connection: c, send: make(chan *Message), pool: pool}
	pool.register <- connection

	go connection.write()
}

var connectionPool = newConnectionPool()

// RunWebsocketPool runs the websocket pool
func RunWebsocketPool() {
	go connectionPool.run()
}
