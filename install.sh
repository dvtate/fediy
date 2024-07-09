#!/bin/bash

# this file is only temporary way to install fediy for testing

sudo rm -rf /opt/fediy
sudo mkdir /opt/fediy
sudo mkdir /opt/fediy/data
sudo mkdir /opt/fediy/data/apps
sudo ln -s "`realpath portal_frontend`" /opt/fediy/data/page_templates
sudo chmod -R 777 /opt/fediy/

touch /opt/fediy/config.ini

cat protocol/db.sql | sqlite3 /opt/fediy/data/db.db3

