# watersense

## Installation iot

Remplir avec vos informations

```c++
const char *serverPassword = "";   // random, needed for server authentification
const char *serverName = "https://apiIp:8000/api/water_senses";

const char *ssid = "";   // The SSID (name) of the Wi-Fsi network you want to connect to
const char *ssidpassword = ""; // The password of the Wi-Fi network
```

Envoyer sur l'esp32 depuis arduino IDE

## Installation API

Modifier le .env avec vos informations

```
composer install
php bin/console doctrine:database:create
php bin/console doctrine:migrations:migrate
symfony serve
```

## Installation Web

Modifier le .env avec vos informations

```
npm install
npm run start
```