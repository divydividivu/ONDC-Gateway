#include <sodium.h>
#include <stdio.h>
#include<string.h>

unsigned char* blake_512hash(const unsigned char * message)
{
    //hashing the reuest body
    unsigned char hash[crypto_generichash_BYTES];
    crypto_generichash(hash, sizeof hash,message, sizeof message,NULL, 0);

    //encoding the digest 
    const size_t bin_len=strlen(hash);
    const size_t b64_maxlen=sodium_base64_ENCODED_LEN(bin_len,sodium_base64_VARIANT_ORIGINAL);
    char* const b64_encoded_digest=malloc(b64_maxlen);
    sodium_bin2base64(b64_encoded_digest, b64_maxlen,hash,bin_len,sodium_base64_VARIANT_ORIGINAL);

    return b64_encoded_digest;
}

char* create_message(char* created,char* expires ,char * request_body)
{
    if (sodium_init() == -1) {
        return -1;
    }

    //hashing the request_body
    char* digest = blake_512hash(request_body);

    //creating message by combining created,expires and digest
    char* msg = (char*) malloc(strlen(created)+strlen(expires)+strlen(digest)+2);
    strcat(msg,created);
    strcat(msg,expires);
    strcat(msg,digest);
    return msg;
}

//function to sign a request using combined signature
unsigned char* sign_request(const unsigned char* msg, unsigned char sk[crypto_sign_SECRETKEYBYTES])
{
    if (sodium_init() == -1) {
        return -1;
    }
    
    
    unsigned char *b64_encoded_signed_message;
    unsigned char signed_message[crypto_sign_BYTES + strlen(msg)];
    unsigned long long signed_message_len;

    //signing the message
    crypto_sign_detached(signed_message,&signed_message_len, msg, strlen(msg), sk);
    //encoding the signature
    const size_t b64_maxlen=sodium_base64_ENCODED_LEN(signed_message_len,sodium_base64_VARIANT_ORIGINAL);
    b64_encoded_signed_message=malloc(b64_maxlen);
    sodium_bin2base64(b64_encoded_signed_message, b64_maxlen,signed_message,signed_message_len,sodium_base64_VARIANT_ORIGINAL);
    
    return b64_encoded_signed_message;
}

//function to verify the signature using combined mode
int verify_request(const unsigned char* b64_encoded_signed_message,const unsigned char* msg, unsigned char pk[crypto_sign_PUBLICKEYBYTES])
{
    if (sodium_init() == -1) {
        return -1;
    }
    
    unsigned char* signed_message = (unsigned char *)malloc(strlen(b64_encoded_signed_message)/4*3);
    const char *b64_end;
    size_t signed_message_len;

    //decoding the signature
    if (sodium_base642bin(signed_message, strlen(b64_encoded_signed_message)/4*3, b64_encoded_signed_message,strlen(b64_encoded_signed_message), "\n\r ", &signed_message_len, &b64_end, sodium_base64_VARIANT_ORIGINAL) != 0) {
        return -1;
    }

    
    unsigned char m[signed_message_len];
    unsigned long long mlen;

    //verifying the signature
    if(crypto_sign_verify_detached(signed_message, msg, strlen(msg), pk)!=0)
        return -1;

    return 1;
}
