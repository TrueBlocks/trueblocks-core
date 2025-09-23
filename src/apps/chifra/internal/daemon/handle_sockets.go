// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package daemonPkg

import (
	"fmt"
	"net"
	"net/http"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/gorilla/websocket"
)

// MessageType is a message carried in a string
type MessageType string

const (
	// CommandErrorMessage is a message send when the server encounters an error
	CommandErrorMessage MessageType = "command_error"
	// CommandOutputMessage is currently not used, but may in the future carry the actual data
	CommandOutputMessage MessageType = "output"
	// ProgressMessage is a message carried on the stderr stream
	ProgressMessage MessageType = "progress"
)

var upgrader = websocket.Upgrader{}

// Message is a structure used to send messages via websockets
type Message struct {
	Action  MessageType `json:"action"`
	ID      string      `json:"id"`
	Content string      `json:"content"`
}

// socketConnection is a structure representing a websocket connection
type socketConnection struct {
	connection *websocket.Conn
	pool       *ConnectionPool
	send       chan *Message
}

// write the message to the connection
func (c *socketConnection) write() {
	defer func() {
		c.connection.Close()
	}()

	// use for-range over the channel
	for message := range c.send {
		err := c.connection.WriteJSON(message)
		if err != nil {
			c.pool.unregister <- c
			break
		}
	}
	c.Log("socketConnection closed")
	_ = c.connection.WriteMessage(websocket.CloseMessage, []byte{})
}

// RemoteAddr is the other end of the connection
func (c *socketConnection) RemoteAddr() net.Addr {
	return c.connection.RemoteAddr()
}

// Log writes a log messages to the server's stderr
func (c *socketConnection) Log(s string, args ...interface{}) {
	subMsg := fmt.Sprintf(s, args...)
	msg := fmt.Sprintf("%s %s", c.RemoteAddr(), subMsg)
	logger.Info(msg)
}

// ConnectionPool is the collection of all connections
type ConnectionPool struct {
	connections map[*socketConnection]bool
	broadcast   chan *Message
	register    chan *socketConnection
	unregister  chan *socketConnection
}

// closeAndDelete cleans up a connection
func closeAndDelete(pool *ConnectionPool, connection *socketConnection) {
	delete(pool.connections, connection)
	close(connection.send)
}

// newConnectionPool returns a new connection structure
func newConnectionPool() *ConnectionPool {
	return &ConnectionPool{
		connections: make(map[*socketConnection]bool),
		broadcast:   make(chan *Message),
		register:    make(chan *socketConnection),
		unregister:  make(chan *socketConnection),
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
	upgrader.CheckOrigin = func(r *http.Request) bool {
		_ = r
		return true
	}

	c, err := upgrader.Upgrade(w, r, nil)
	if err != nil {
		logger.Error("websocket upgrade failed", err)
		return
	}

	connection := &socketConnection{
		connection: c,
		send:       make(chan *Message),
		pool:       pool,
	}
	pool.register <- connection

	go connection.write()
}

var connectionPool = newConnectionPool()

// RunWebsocketPool runs the websocket pool
func RunWebsocketPool() {
	go connectionPool.run()
}
