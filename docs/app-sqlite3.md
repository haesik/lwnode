# Sqlite3 App Guide

In this guide, we will show you how to write a `hello world` sqlite3 app. To do so, two apps are required, one for running a Database service on a server, and a client app, which accesses to the Database service. A server is usually a Tizen device, such as a Tizen TV, and a client is typically a Tizen app. The Database service runs on top of a Web server, which is `express.js` in our framework.

## 1. Run Sqlite3 App on a Server

### 1.1. Build and install a Sqlite3 server app
[Build lwnode rpms](BUILD.md) and
install `lwnode-sqlite3.armv7l.rpm` on your device. The rpms can also be downloaded from [tizen.org](https://download.tizen.org/snapshots/tizen/unified/latest/repos/standard/packages/armv7l/).
```sh
$ rpm -Uvh lwnode-sqlite3.armv7l.rpm
```

### 1.2. Run the Sqlite3 server app
Run the Sqlite3 server app on your device.
```sh
$ lwnode /usr/apps/lwnode/apps/sqlite3/script
```

We use `lwnode` to run a `express` server, which in turn, runs the Sqlite3 server. When successful, a message below is printed.
```sh
[xx:xx:xx][INFO] listening port: 8140
```


## 2. Run a client Sqlite3 app
### 2.1. Download `libsqlite.js`
Download `libsqlite.js` from `http://<your device ip>:8140/api/download/libsqlite.js`. A Web app client will use this library to communicate with the Server.
```sh
$ wget http://<your device ip>:8140/api/download/libsqlite.js
```

### 2.2. Add `libsqlite.js` library to your Web app
Add the library to your Tizen Web app. This library provides Database API required to commnunite with the server.

```html
<!-- index.html -->
<script src="./libsqlite.js"></script>
```

### 2.3. Run database operations on the client
To use Sqlite3 in your Web app, use the `sqlite3` object that is defined in `global`.

The following is an example of creating a table.

```js
// main.js
const DB_HOSTNAME = '127.0.0.1';
const DB_PORT = 8140;
const DB_PATH = '/';
const url = `ws://${DB_HOSTNAME}:${DB_PORT}${DB_PATH}`;

window.onload = async function () {
  sqlite3.configure(url);

  // use the sqlite3 object to access to Database service
  let db = await new sqlite3.Database(':memory:', sqlite3.OPEN_READWRITE | sqlite3.OPEN_CREATE);

  const q = `CREATE TABLE user (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        name text,
        email text UNIQUE,
        password text,
        CONSTRAINT email_unique UNIQUE (email)
        );`;
  try {
    let result = await db.run(q);
    console.log('success', result);
  } catch (err) {
    console.log('fail');
    throw err;
  }
};
```

## How to Use express
If you want to use express, edit the file below on your device.
```bash
/usr/apps/lwnode/apps/sqlite3/script/userapp.js
```
You will see `startUserApp` function. Implement your app here.
```js
// /usr/apps/lwnode/apps/sqlite3/script/userapp.js
function startUserApp(app) {
  // params
  // app: app object of express
}
```