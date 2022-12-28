#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <json-c/json.h>

void replay_check(char* json_str)
{
    json_object *json = json_tokener_parse(json_str);
    
    //TODO : check for trans id in the db (if already used or not)

    // Extract the "context" object
    json_object *context;
    json_object_object_get_ex(json, "context", &context);

    // Extract the timestamp and TTL strings
    json_object *timestamp_str, *ttl_str;
    json_object_object_get_ex(context, "timestamp", &timestamp_str);
    json_object_object_get_ex(context, "ttl", &ttl_str);

    // Convert the timestamp and TTL strings to their corresponding time_t values
    struct tm tm;
    char *format_str = "%Y-%m-%dT%H:%M:%S.%fZ";
    strptime(json_object_get_string(timestamp_str), format_str, &tm);
    time_t timestamp = mktime(&tm);
    
    time_t ttl = 0;
    sscanf(json_object_get_string(ttl_str), "PT%ldS", &ttl);
    printf("Timestamp: %ld, TTL: %ld\n", timestamp, ttl);

    printf("Timestamp + TTL: %ld\n", timestamp + ttl);

    printf("Current time: %ld\n", time(NULL));
    // Check if the timestamp plus the TTL is greater than the current timestamp
    time_t now = time(NULL);
    time_t max_time = timestamp + ttl;
    if (max_time > now) {
        printf("No replay attack detected.\n");
        
    } else {
        printf("Possible replay attack detected!\n");
    }
}
