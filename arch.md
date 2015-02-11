# central server

In the poc we need a server that accepts message submissions and writes them to
disc so they can be rsynced by clients.

 * Python / wsgi based server (gunicorn?) that exposes a single HTTP post endpoint for accepting a message
 * Writes out message to the filesystem
 * filename for message is base64 encoding of sha256 of message content
 * Puppet code for setting up the above + rsync daemon

# local client

 * compose command
  * accept username that maps to keybase.io api
  * accept message
  * encode in wire format / encrypt with keybased pubkey / send to http api of central server
 * fetch command
  * rsync all messages from central server to local disk
  * attept to decrypt all new messages
  * flag which are decryptable
 * read command
  * keep track of which "flagged decryptable" messages have been read
  * display list of "flagged decryptable" and allow user to read them
