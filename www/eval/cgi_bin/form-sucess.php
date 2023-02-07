<!doctype html>
<html lang="en-us">

    <head>

        <!-- Meta -->
        <meta charset="utf-8">
        <meta http-equiv="x-ua-compatible" content="ie=edge">
        <meta name="viewport" content="width=device-width,initial-scale=1">

        <title>Webserver 42</title>
        <meta name="description" content="">

        <!-- The compiled CSS file -->
        <link rel="stylesheet" href="css/production.css">
        <link rel="stylesheet" href="css/custom.css">

        <!-- Web fonts -->
        <link href="https://fonts.googleapis.com/css?family=Space+Mono:400,700" rel="stylesheet">

        <!-- favicon.ico. Place these in the root directory. -->
        <link rel="shortcut icon" href="favicon.ico">

    </head>

    <body class="has-animations">


    <!-- Create outer border -->
    <div class="page-border">
    <div class="bg--white">

        <!-- Header -->
        <header class="align--center pt3 pb2">
            <div class="container">
                <div class="margin-bottom-42">
                    <iframe class="magin-bottom-42" src="https://embed.lottiefiles.com/animation/135006"></iframe>
                    <?php
                    $f = fopen( 'php://stdin', 'r' );
                    $finalLine = "";

                    while( $line = fgets( $f ) ) {
                        $finalLine .= $line;
                    }
                    fclose( $f );
                    
                    $data = []; 
                    parse_str($finalLine, $data);
                    
                    // Display the data recieved:
                    // echo "The final line: " . $finalLine . "<br>";
                    // echo "The Array: " . "<br>";
                    // var_dump( $data )

                    ?>
                    <h2 class="mb3 p-60 magin-bottom-42 reveal-on-scroll is-revealing">Hello <?php echo $data['firstname']?>! I'm a PHP file! Thanks for all the data you sent me! I will keep them safe and warm!</h2>
                    <div>
                        <p class="">My dear <?php echo $data['firstname']?> <?php echo $data['lastname']?>! You are <?php echo $data['age']?> years old and you tell me this story:</p>
                        <p class=""><?php echo $data['story']?></p>
                        <p class="">That was a fantastic story! Did you ever thought about being an Author Like J.K Rowling? You Should!<br></p>
                    </div>
                    <div class="magin-bottom-42">
                        <p class="mb1 p-60">By the way here are some extra data I recieved:</p>
                        <p><?php echo 'Server name: ' . $_SERVER['SERVER_NAME']; ?></p>
                        <p><?php echo 'Server protocol: ' . $_SERVER['SERVER_PROTOCOL']; ?></p>
                        <p><?php echo 'Server port: ' . $_SERVER['SERVER_PORT']; ?></p>
                        <p><?php echo 'Request method: ' . $_SERVER['REQUEST_METHOD']; ?></p>
                        <p><?php echo 'scrip name: ' . $_SERVER['SCRIPT_NAME']; ?></p>
                        <p><?php echo 'query string: ' . $_SERVER['QUERY_STRING']; ?></p>
                        <p><?php echo 'gateway interface: ' . $_SERVER['GATEWAY_INTERFACE']; ?></p>
                        <p><?php echo 'Content type: ' . $_SERVER['CONTENT_TYPE']; ?></p>
                        <p><?php echo 'Content length: ' . $_SERVER['CONTENT_LENGTH']; ?></p>
                        <p><?php echo 'Http accept: ' . $_SERVER['HTTP_ACCEPT']; ?></p>
                        <p><?php echo 'Languages: ' . $_SERVER['HTTP_ACCEPT_LANGUAGE']; ?></p>
                        <p><?php echo 'User agent: ' . $_SERVER['HTTP_USER_AGENT']; ?></p>
                    </div>
                </div>
                <a href="/" class="btn btn--outline reveal-on-scroll is-revealing">Go back to index</a>
            </div>
        </header>

        <!-- Footer -->
        <footer class="mt2 pb3 align--center">
            <p class="mt1 small text--gray p-60">We use a template made by <a href="https://www.papayatemplates.com" class="link">Papaya </a>because the focus of this project is webserver and not creating a website.</p>
        </footer>

    </div>
    </div>

    <!-- Scroll reveal -->
    <script src="https://unpkg.com/scrollreveal@4.0.0/dist/scrollreveal.min.js"></script>

    <!-- The compiled JavaScript file -->
    <script src="js/production.js"></script>

    </body>
</html>

