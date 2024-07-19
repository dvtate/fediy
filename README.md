# Protocol + Federated Git

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

## Components
Eventually these will all get moved to separate repos
- Protocol Server:
    - User/admin portal: account creation and management, app list, server management?, etc.
    - Module/app management:
    - Peer/Authenticaiton:
- Shop:
    - How users are able to find and pay for mods
    - Would be cool if they had a
- Apps:
    - fediymod.h: compile a .so file for your mod using interface provided
    - 
    - Simple chat app/social network/blog?
    -
- Landing page
- Supporting sites:
    - Git adopt
    - Modnet to handle platform abuse

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
