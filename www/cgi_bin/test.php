<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Cgi hello</title>
</head>
<body>
    <h1><?php echo "Hello from cgi" ?></h1>
    <p><?php echo 'Server name: ' . $_SERVER['SERVER_NAME']; ?></p>
    <p><?php echo 'Server protocol: ' . $_SERVER['SERVER_PROTOCOL']; ?></p>
    <p><?php echo 'Server port: ' . $_SERVER['SERVER_PORT']; ?></p>
    <p><?php echo 'Request method: ' . $_SERVER['REQUEST_METHOD']; ?></p>
    <p><?php echo 'scrip name: ' . $_SERVER['SCRIPT_NAME']; ?></p>
    <p><?php echo 'query string: ' . $_SERVER['QUERY_STRING']; ?></p>
    <p><?php echo 'remote host: ' . $_SERVER['REMOTE_HOST']; ?></p>
    <p><?php echo 'gateway interface: ' . $_SERVER['GATEWAY_INTERFACE']; ?></p>
    <p><?php echo 'Content type: ' . $_SERVER['CONTENT_TYPE']; ?></p>
    <p><?php echo 'Content length: ' . $_SERVER['CONTENT_LENGTH']; ?></p>
    <p><?php echo 'Http accept: ' . $_SERVER['HTTP_ACCEPT']; ?></p>
    <p><?php echo 'Languages: ' . $_SERVER['HTTP_ACCEPT_LANGUAGE']; ?></p>
    <p><?php echo 'User agent: ' . $_SERVER['HTTP_USER_AGENT']; ?></p>
</body>
</html>
