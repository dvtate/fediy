# Federated Git

## Big Features/Selling points
- Data ownership
- Collaboration (ie - git)
- Single Sign On (SSO)
- Sever app management

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
- Kanban-IY: jira/trello/etc. killer



Architecture?:
- Platform/protocl:
  - manage apps (add, remove, change settings), users
  - acts as a middleman between apps that handles authentication
  - communicates with the servers
- Library: utils for building and interacting with apps on the platform
- Apps: use library to handle fediverse to gain access to authentication and stuff
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
