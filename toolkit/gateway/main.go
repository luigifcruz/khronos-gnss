package main

import (
	fmt "fmt"
	"io/ioutil"
	"log"
	"os"
	"time"

	"github.com/golang/protobuf/proto"
	"github.com/tarm/serial"
)

func makeTimestamp() uint64 {
	return uint64(time.Now().UnixNano()) / uint64(time.Millisecond)
}

func main() {
	//payload := "These violent delights have violent ends\nAnd in their triumph die, like fire and powder,\nWhich, as they kiss, consume. The sweetest honey\nIs loathsome in his own deliciousness\nAnd in the taste confounds the appetite.\nTherefore love moderately. Long love doth so.\nToo swift arrives as tardy as too slow."

	file, err := os.Open("/home/luigifcruz/Downloads/nrzm.py")
	b, err := ioutil.ReadAll(file)

	if err != nil {
		log.Fatal(err)
	}

	p := &Shuttle{
		Sender:      "PU2SPY",
		Receiver:    "PU2NVX",
		Timestamp:   makeTimestamp(),
		Flightstamp: makeTimestamp() + 2000,
		Type:        00,
		File: &File{
			Name: "/spiffs/blackhole.jpeg",
			Blob: []byte(b),
		},
	}
	fmt.Println(p)

	data, err := proto.Marshal(p)
	if err != nil {
		log.Fatal("Marshal Error!", err)
	}

	fmt.Println(len(data))
	fmt.Println(data)
	ioutil.WriteFile("./shuttle.pb", data, 0644)

	c := &serial.Config{Name: "/dev/ttyUSB0", Baud: 9600}
	s, err := serial.OpenPort(c)
	if err != nil {
		log.Fatal(err)
	}

	_, err = s.Write(append(data, []byte{0x1a, 0xcf, 0xfc, 0x1d}...))
	if err != nil {
		log.Fatal(err)
	}

	fmt.Println("AuLoRa Shuttle Sent!")
}
