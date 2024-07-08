# Federated Git

## Big Features
- Sever app management
- SSO
- Collaboration (ie - git)

## App Ideas
- Git-IY: GitLab killer 
- Blog-IY: Wordpress killer
- Chat-IY: Matrix competitor
  - text, vc, teams, etc.
- Social-IY: Mastodon competitor
- Contacts: Rolodex
- Wiki-IY: mediawiki killer
- Edit-IY: Google docs killer
- Vid-IY: peertube competitor (integrate with grayjay)
- Calendar-IY: Google Calendar killer
- Cloud-IY: Nextcloud competitor



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



# Data Directory
```
    data_dir/
        db.db3      - sqlite database
        pubkey      -
        config.ini  - global admin settings

        apps/
            example.app.id/
                config.json
                index.so
```



# Security
- specific user for app but run modules as a different user
