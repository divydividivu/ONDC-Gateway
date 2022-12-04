#include <sodium.h>
#include <stdio.h>
#include<string.h>
#define MESSAGE (const unsigned char *) "testeddddddddddddddd"
#define MESSAGE_LEN 20

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
    char* msg ="";
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
    unsigned char signed_message[crypto_sign_BYTES + MESSAGE_LEN];
    unsigned long long signed_message_len;

    //signing the message
    crypto_sign(signed_message, &signed_message_len,MESSAGE, MESSAGE_LEN, sk);

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
    if(crypto_sign_open(m, &mlen,signed_message,signed_message_len, pk)!=0)
        return -1;

    if (mlen != MESSAGE_LEN || strcmp(msg, m) != 0) {
        return -1;
    }
    return 0;
}

int main()
{
     if (sodium_init() == -1) {
        return -1;
    }

    unsigned char pk[crypto_sign_PUBLICKEYBYTES];
    unsigned char sk[crypto_sign_SECRETKEYBYTES];
    crypto_sign_keypair(pk, sk);

    unsigned long long signed_message_len;
    unsigned char* signed_message = (unsigned char*)malloc(strlen(sign_request(MESSAGE,sk)));

    //signing the request
    strcpy(signed_message,sign_request(MESSAGE,sk));

    unsigned char unsigned_message[MESSAGE_LEN];
    unsigned long long unsigned_message_len;

    printf("signed message: %s\n",signed_message);
    printf("hashed signed_message: %s\n",blake_512hash(signed_message));
    if (verify_request(signed_message,MESSAGE,pk) != 0) {
        /* incorrect signature! */
        printf("incorrect signature\n");
    }
    else
        printf("correct signature\n");
}