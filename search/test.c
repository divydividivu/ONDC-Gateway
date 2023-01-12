#include <bson/bson.h>
#include<json-c/json.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  char *json_string = "{'key': 'value'}";
struct json_object *parsed;
parsed = json_tokener_parse(json_string);
bson_t *bson = bson_new_from_json(json_object_to_json_string(parsed), -1, NULL);

    char *json_output = bson_as_json(bson, NULL);
    printf("BSON data as JSON: %s\n", json_output);
    bson_free(json_output);

    json_object_put(parsed);
    bson_destroy(bson);

    return 0;
}
