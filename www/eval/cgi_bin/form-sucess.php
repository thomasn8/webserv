<!doctype html>
<html lang="en-us">

    <head>

        <!-- Meta -->
        <meta charset="utf-8">
        <meta http-equiv="x-ua-compatible" content="ie=edge">
        <meta name="viewport" content="width=device-width,initial-scale=1">

        <title>Webserver 42</title>
        <meta name="description" content="">

        <!-- favicon -->
        <link rel="apple-touch-icon" sizes="180x180" href="/favicon_package/apple-touch-icon.png">
        <link rel="icon" type="image/png" sizes="32x32" href="/favicon_package/favicon-32x32.png">
        <link rel="icon" type="image/png" sizes="16x16" href="/favicon_package/favicon-16x16.png">
        <link rel="manifest" href="/favicon_package/site.webmanifest">
        <link rel="mask-icon" href="/favicon_package/safari-pinned-tab.svg" color="#5bbad5">

        <!-- The compiled CSS file -->
        <link rel="stylesheet" href="../css/production.css">
        <link rel="stylesheet" href="../css/custom.css">

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
                    <iframe class="margin-bottom-42" src="https://embed.lottiefiles.com/animation/135006"></iframe>
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
                    <?php if (empty($data['firstname']) || empty($data['firstname']) || empty($data['age']) || empty($data['story'])) : ?>
                        <?php if (empty($data['firstname'])) : ?>
                            <h2 class="mb3 p-60 margin-bottom-42 reveal-on-scroll is-revealing">You forget to enter your name!</h2>
                        <?php endif; ?>
                        <?php if (empty($data['lastname'])) : ?>
                            <h2 class="mb3 p-60 margin-bottom-42 reveal-on-scroll is-revealing">You forget to enter your lastname!</h2>
                        <?php endif; ?>
                        <?php if (empty($data['age'])) : ?>
                            <h2 class="mb3 p-60 margin-bottom-42 reveal-on-scroll is-revealing">You forget to enter your age!</h2>
                        <?php endif; ?>
                        <?php if (empty($data['story'])) : ?>
                            <h2 class="mb3 p-60 margin-bottom-42 reveal-on-scroll is-revealing">You forget to tell me a story!</h2>
                        <?php endif; ?>
                        <a href="/form-cgi.html" class="btn btn--outline reveal-on-scroll is-revealing">Go back to the form</a>
                    <?php else: ?>
                        <h2 class="mb3 p-60 margin-bottom-42 reveal-on-scroll is-revealing">Hello <?php echo $data['firstname']?>! I'm a PHP file! Thanks for all the data you sent me! I will keep them safe and warm!</h2>
                        <div class="margin-bottom-42">
                            <p class="p-60">My dear <?php echo $data['firstname']?> <?php echo $data['lastname']?>! You are <?php echo $data['age']?> years old and you told me this story:</p>
                            <p class="story p-60"><?php echo $data['story']?></p>
                            <p class="p-60">That was a fantastic story! Did you ever thought about being an Author Like J.K Rowling? You Should!<br></p>
                        </div>
                        <div class="margin-bottom-42 align-left p-60">
                            <h3 class="mb1 margin-bottom-42">By the way here are some extra data I recieved:</h3>
                            <p><strong>Server name: </strong><?php echo $_SERVER['SERVER_NAME']; ?></p>
                            <p><strong>Server protocol:: </strong><?php echo $_SERVER['SERVER_PROTOCOL']; ?></p>
                            <p><strong>Server port: </strong><?php echo $_SERVER['SERVER_PORT']; ?></p>
                            <p><strong>Request method: </strong><?php echo $_SERVER['REQUEST_METHOD']; ?></p>
                            <p><strong>scrip name: </strong><?php echo $_SERVER['SCRIPT_NAME']; ?></p>
                            <p><strong>query string: </strong><?php echo $_SERVER['QUERY_STRING']; ?></p>
                            <p><strong>gateway interface: </strong><?php echo $_SERVER['GATEWAY_INTERFACE']; ?></p>
                            <p><strong>Content type: </strong><?php echo $_SERVER['CONTENT_TYPE']; ?></p>
                            <p><strong>Content length: </strong><?php echo $_SERVER['CONTENT_LENGTH']; ?></p>
                            <p><strong>Http accept: </strong><?php echo $_SERVER['HTTP_ACCEPT']; ?></p>
                            <p><strong>Languages: </strong><?php echo $_SERVER['HTTP_ACCEPT_LANGUAGE']; ?></p>
                            <p><strong>User agent: </strong><?php echo $_SERVER['HTTP_USER_AGENT']; ?></p>
                        </div>
                    <?php endif; ?>
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
    <script src="../js/production.js"></script>

    </body>
</html>

