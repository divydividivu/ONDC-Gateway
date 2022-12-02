#include<stdio.h>
#include<json-c/json.h>

int main()
{
    FILE *fp;
    char *buffer = malloc(100000);

    fp = fopen("test.json", "r");
    fread(buffer, 100000, 1, fp);
    fclose(fp);

    struct json_object *parsed;
    parsed = json_tokener_parse(buffer);
    
    struct json_object *context;
    json_object_object_get_ex(parsed, "context", &context);
    
    
    struct json_object *domain;
    struct json_object *country;
    struct json_object *city;
    struct json_object *action;
    struct json_object *core_version;
    struct json_object *bap_id;
    struct json_object *bap_uri;
    struct json_object *bpp_id;
    struct json_object *bpp_uri;
    struct json_object *transaction_id;
    struct json_object *message_id;
    struct json_object *timestamp;
    struct json_object *key;
    struct json_object *ttl;

    json_object_object_get_ex(context, "domain", &domain);
    json_object_object_get_ex(context, "country", &country);
    json_object_object_get_ex(context, "city", &city);
    json_object_object_get_ex(context, "action", &action);
    json_object_object_get_ex(context, "core_version", &core_version);
    json_object_object_get_ex(context, "bap_id", &bap_id);
    json_object_object_get_ex(context, "bap_uri", &bap_uri);
    json_object_object_get_ex(context, "bpp_id", &bpp_id);
    json_object_object_get_ex(context, "bap_uri", &bpp_uri);
    json_object_object_get_ex(context, "transaction_id", &transaction_id);
    json_object_object_get_ex(context, "message_id", &message_id);
    json_object_object_get_ex(context, "timestamp", &timestamp);
    json_object_object_get_ex(context, "key", &key);
    json_object_object_get_ex(context, "ttl", &ttl);

    printf("Domain: %s\n", json_object_get_string(domain));
    printf("Country: %s\n", json_object_get_string(country));
    printf("City: %s\n", json_object_get_string(city));
    printf("Action: %s\n", json_object_get_string(action));
    printf("Core Version: %s\n", json_object_get_string(core_version));
    printf("BAP ID: %s\n", json_object_get_string(bap_id));
    printf("BAP URI: %s\n", json_object_get_string(bap_uri));
    printf("BPP ID: %s\n", json_object_get_string(bpp_id));
    printf("BPP URI: %s\n", json_object_get_string(bpp_uri));
    printf("Transaction ID: %s\n", json_object_get_string(transaction_id));
    printf("Message ID: %s\n", json_object_get_string(message_id));
    printf("Timestamp: %s\n", json_object_get_string(timestamp));
    printf("Key: %s\n", json_object_get_string(key));
    printf("TTL: %s\n", json_object_get_string(ttl));
    
    free(buffer);
}