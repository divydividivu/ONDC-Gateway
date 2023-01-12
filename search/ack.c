#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <json-c/json.h>
#include <string.h>

char* form_ack_msg(char* body, int is_ack)
{
  json_object *json = json_tokener_parse(body);
  json_object *context = json_object_object_get(json, "context");
  
  json_object *request = json_object_new_object();
  json_object_object_add(request, "request", json);
  json_object *response = json_object_new_object();
  json_object_object_add(response, "response", context);
  json_object *message = json_object_new_object();
  json_object *ack = json_object_new_object();
  json_object *status = json_object_new_string("NACK");
  if (is_ack)
  {
    status = json_object_new_string("ACK");
  }
  
  json_object_object_add(ack, "status", status);
  json_object_object_add(message, "ack", ack);
  json_object_object_add(response, "message", message);

  json_object *ack_message = json_object_new_object();
  json_object_object_add(request, "response", json_object_get(response));
  json_object_object_add(ack_message, "search", request);
  return ack_message;
}

void send_ack(char* body, int client_fd, int is_ack)
{

  // Convert the JSON object to a string and allocate memory for it

  char *json_string = strdup(json_object_to_json_string_ext(form_ack_msg(body, is_ack), JSON_C_TO_STRING_PLAIN));
  int json_length = strlen(json_string);

  // Concatenate the JSON string to the response
  char* response = malloc(strlen("HTTP/1.1 200 OK\r\n\r\n ") + json_length + 1);
  strcpy(response, "HTTP/1.1 200 OK\r\n\r\n ");
  strncat(response, json_string, json_length);

  // Send the response
  int sent = 0;
  int total_sent = 0;
  int response_length = strlen(response);
  while (total_sent < response_length)
  {
    sent = send(client_fd, response + total_sent, response_length - total_sent, 0);
    if (sent == -1)
    {
      // Handle the error
      break;
    }
    total_sent += sent;
  }

  // Clean up
  free(response);
  free(json_string);

}

