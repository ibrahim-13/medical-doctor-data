# **Blockchain based Data storage and access** #

## *This is a demonstrative application that is Blockchain based approach to store data securely and access them based on search parameters.* ##

## Frameworks and dependencies :##
*1. AngularJS :: For front-end*
*2. Qt :: For C++ back-end server*
*3. nginx :: For primary server*
*4. mbedTLS :: For SHA-512 hashing*

Source of the back-end server is located in **backend_server** folder.
A demonstrative distribution is located in the **dist** folder.

Primary server is **nginx** and it is configured to work as reverse proxy to redirect requests
from **"/api/blockchain"** to the back-end server. nginx is used for file server.

The back-end server only replies JSON formatted data.