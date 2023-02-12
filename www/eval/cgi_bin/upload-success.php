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
                    <?php
                        $f = fopen( 'php://stdin', 'r' );
                        $finalLine = "";

                        while( $line = fgets( $f ) ) {
                            $finalLine .= $line;
                        }
                        fclose( $f );
                        
                        $data = []; 
                        parse_str($finalLine, $data);
                        // var_dump( $data )
                    ?>
                    <iframe class="magin-bottom-42" src="https://embed.lottiefiles.com/animation/135006"></iframe>
                    <h2 class="mb3 reveal-on-scroll is-revealing">You sucessfully uploaded a file!</h2>
                    <p class="mb1 p-60"><strong>file: </strong><?php echo $data['my_upload']?></p>
                    <p class="mb1 p-60">You can see it in the uploads directory in the server.</p>
                    <p class="mb1 p-60">Now it's time to delete this file.</p>
                    <a href="https://web.postman.co/" target="_blank" class="btn btn--outline reveal-on-scroll is-revealing">Oki doki!</a>
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

