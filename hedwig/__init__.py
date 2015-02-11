import requests
import sys
from tempfile import NamedTemporaryFile

# TODO make configurable
HEDWIG_PATH = "~/.hedwig"
SERVER_HOST = "dne.local:3443"
USAGE = "lol not a command"

def mktempfile():
    return NamedTemporaryFile().name

def fetch_key(recipient):
    """Given the keybase.io screenname of a user, fetch their public
    key from keybase.io, strip headers, return as concatenated
    string"""
    return "TODO"

def pack_msg(content, recipient_key, sender_key):
    """given some plaintext message content, a recipient's pubkey
    (from keybase) and sender's pubkey (from configuration), shell out
    to the message packer and return the base64 encoded response"""
    return "TODO"

def send(packed_content):
    """given content processed / packed by the external tool, send it
    to the central server's http api"""
    return "TODO"

def compose(*args):
    recipient = raw_input("Recipient: ")
    recipient_key = fetch_key(recipient)
    tmp_file = mktempfile()
    # shell out to $EDITOR, pass tmp_file
    # read in tmp_file
    # pack contents of tmp_file
    # submit to central server
    return "compose"

def fetch(*args):
    print(args)
    return "fetch"

def read(*args):
    print(args)
    return "read"

def dispatch(command, args):
    commands = {
        'compose': compose,
        'fetch': fetch,
        'read': read,
    }
    command_fn = commands.get(command, lambda *a: USAGE)
    print(command_fn(*args))

if __name__ == '__main__':
    command = sys.argv[1]

    dispatch(command, sys.argv[2:])
