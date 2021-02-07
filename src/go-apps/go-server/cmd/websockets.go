package trueblocks

import (
	"log"
	"net/http"

	"github.com/gorilla/websocket"
)

type MessageType string;

const (
	CommandErrorMessage MessageType = "command_error"
	CommandOutputMessage MessageType = "output"
	ProgressMessage MessageType = "progress"
)

var upgrader = websocket.Upgrader{}

type Message struct {
	Action MessageType `json:"action"`
	Id string `json:"id"`
	Content string `json:"content"`
	Progress *CommandProgress `json:"progress"`
}

type Connection struct {
	connection *websocket.Conn
	send chan *Message
}

func (c *Connection) write() {

	defer func() {
		c.connection.Close()
	}()

	for {
		select {
		case message, ok := <- c.send:
			if !ok {
				log.Printf("Connection closed")
				c.connection.WriteMessage(websocket.CloseMessage, []byte{})
				return
			}

			err := c.connection.WriteJSON(message)

			if err != nil {
				log.Fatal(err)
			}
		}
	}
}

type ConnectionPool struct {
	connections map[*Connection]bool
	broadcast chan *Message
	register chan *Connection
	unregister chan *Connection
}

func closeAndDelete(pool *ConnectionPool, connection *Connection) {
	delete(pool.connections, connection)
	close(connection.send)
}

func newConnectionPool() *ConnectionPool {
	return &ConnectionPool{
		connections: make(map[*Connection]bool),
		broadcast: make(chan *Message),
		register: make(chan *Connection),
		unregister: make(chan *Connection),
	}
}

func (pool *ConnectionPool) run() {
	for {
		select {
		case connection := <- pool.register:
			pool.connections[connection] = true
		case connection := <- pool.unregister:
			if _, ok := pool.connections[connection]; ok {
				closeAndDelete(pool, connection)
			}
		case message := <- pool.broadcast:
			for connection := range pool.connections {
				connection.send <- message
			}
		}
	}
}

func HandleWebsockets(pool *ConnectionPool, w http.ResponseWriter, r *http.Request) {
	c, err := upgrader.Upgrade(w, r, nil)
	if err != nil {
		log.Print("upgrade:", err)
		return
	}

	connection := &Connection{connection: c, send: make(chan *Message)}
	pool.register <- connection

	go connection.write()
}
