## Twelve Sky 2.5 Project Installation Guide

This guide will help you set up your Twelve Sky 2.5 server project on an Ubuntu-based server. Follow these steps carefully.

### 1. Installation and Configuration of MySQL Database

First, install MySQL and configure it securely:

```bash
sudo apt update
sudo apt install mysql-server
sudo mysql_secure_installation
```

Create a MySQL database and grant user permissions:

```bash
sudo mysql -p
CREATE USER 'db_username_12sky2'@'%' IDENTIFIED BY 'YourPassword';
GRANT ALL PRIVILEGES ON * . * TO 'db_username_12sky2'@'%';
FLUSH PRIVILEGES;
quit
```

Replace `'db_username_12sky2'` and `'YourPassword'` with your desired database username and password.

### 2. Installing Required Packages

Install the necessary packages:

```bash
sudo apt-get update
sudo apt-get install build-essential
sudo apt-get install libmysqld-dev
```

### 3. Compiling Twelve Sky 2.5 Server Source Code

Navigate to your project folder and compile the components, starting with TS2_CENTER:

```bash
cd /user/source/TS2_CENTER/src
make clean
make
make install
```

Repeat the above steps for TS2_EXTRA, TS2_GAMELOG, TS2_LOGIN, TS2_PLAYUSER, TS2_RELAY, and TS2_ZONE:

```bash
cd /user/source/TS2_EXTRA/src
make clean
make
make install

# Repeat the same steps for other server components.
```

### 4. Compiling and Installing the zlib Library

Compile and install the zlib library:

```bash
cd /user/source/zlib-1.2.3
make clean
make
make install
```

### 5. Server Configuration

Navigate back to your project folder and run the configuration script:

```bash
source Build
cd /home/guitar/12sky/twelve-sky/server/service/etc
sh config_gen.sh
```

### 6. Server Controls

To start the server, use the following command:

```bash
cd /user/
sh start.sh
```

To stop the server, use the following command:

```bash
cd /user/
sh stop.sh
```

### 7. GM Authorization

To grant GM privileges, access MySQL again:

```bash
sudo mysql -p
GRANT ALL PRIVILEGES ON * . * TO 'db_username_12sky2'@'%' WITH GRANT OPTION;
FLUSH PRIVILEGES;
quit
```

Replace `'db_username_12sky2'` with your database username.

### Notes

- Replace `'db_username_12sky2'` and `'YourPassword'` with your actual database username and password.
- Pay close attention to security and permissions, and configure your server appropriately.
- Keep your server and database passwords secure, and use strong passwords.

## License

This project is licensed under the [MIT License](LICENSE).

## Issues, Feature Requests or Support

Please use the [New Issue](https://github.com/akinbicer/TS25v1/issues/new) button to submit issues, feature requests or support issues directly to me. You can also send an e-mail to akin.bicer@outlook.com.tr.
