# git-fed

# Federated Git

Git FS manager:
- handles local copies of git repos
- secure communications with the other instances
	- clone repos from other instances
	- 

Interinstance manager:
- establishes connections between instances
- created auth credentials


Endpoints:
- Auth:
	- SSL/Key?
- Frontend: For repos:
	- For local users, this will be generated from local repo, for remote users this will come from
	- summary: files list
	- 
	- files summary 


Architecture?:
- Platform: manage apps (add, remove, change settings), users, federation, etc.
- Library: utils for interacting with other apps on the platform
- Apps: link with libray to gain access to authentication and stuff
	- Apps are run and stopped by the platform


MVP:
- authentication
- module loading
- hosted repos
- limited/no gui



# Peer Authentication:
## Process
- get peer's public key from endpoint
	- /auth/pub -> peer.key
- encrypted with peer.key, we send our public key, domain, and token to peer ; request signed with our pubkey
	- /auth/connect -> auth token, symmetric key
	- peer checks that provided public key corresponds with our /auth/pub key
	- peer sends back encrypted token along with symmetric key
- 

- peer gets public certificate from server
- peer sends request to get signed token from peer
	- peer can reject this request
- 




# Security
- specific user for app but run modules as a different user
