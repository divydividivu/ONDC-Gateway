package main

import "C"
import (
   "io/ioutil"
   "log"
   "net/http"
)

//export get
func get() *C.char {
	resp, err := http.Get("https://jsonplaceholder.typicode.com/posts")
	if err != nil {
      log.Fatalln(err)
	}
	
	body, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		log.Fatalln(err)
	}

	var cstr *C.char 

	sb := string(body)
	cstr = C.CString(sb)

	return cstr
}

func main() {
}