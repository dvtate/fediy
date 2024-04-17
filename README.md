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