# Use Case
lookup common password hashes from a plain password something like (rockyou.txt)[https://weakpass.com/wordlists/rockyou.txt]

# Quick Setup
1) Prerequisites:
```bash
sudo apt install openssl libssl-dev
```

2) Building:
```bash
make
```

3) Example:

- get rockyou.txt.gz:
```bash
wget "https://weakpass.com/download/90/rockyou.txt.gz"
gzip -dk rockyou.txt.gz
```

- run the program
```bash
./main.out ./rockyou.txt $(echo -n "password123" | sha1sum | awk '{print $1}') SHA1
```

