# Example
prerequisites: OpenSSL, a wordlist (e.g. [rockyou.txt](https://weakpass.com/wordlists/rockyou.txt)) 
to generate hashes from plaintext passwords for comparison.

```bash
make && ./main.out ./rockyou.txt $(echo -n "password123" | sha1sum | awk '{print $1}') SHA1
```
