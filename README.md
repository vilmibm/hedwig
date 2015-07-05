# hedwig

## Message Format

Sender sig of MAC
MAC
AES key, encrypted to recipient
Recipient fingerprint, AES encrypted
Sender fingerprint, AES encrypted
Body, AES encrypted

## Decryption Process

* Stash MAC signature and MAC somewhere
* verify MAC on encrypted data
* Decrypt AES key with our private RSA key
* Decrypt recipient fingerprint with AES key
  * If not our fingerprint, not our message. Skip the rest of this process.
  * If our finterprint, our message. Continue.
* Decrypt sender fingerprint with AES key
* Verify we trust the pub key for that fingerprint
  * If not, alert the user that this message isn't trusted, but continue
* Decrypt message body and present to user

## Encryption Process

* Generate an AES key
* concatenate recipient fingerprint, sender fingerprint, and message
* encrypt that junk with the AES key
* concatenate AES key with encrypted blob
* Hash that shit up
* Sign the hash
* Concatenate your signature, the hash, and the previous concatenated stuff
