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

		<script src="https://cdnjs.cloudflare.com/ajax/libs/jquery/3.3.1/jquery.min.js"></script>

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
                    
                    <?php if (!empty($data['filename'])) : ?>

						<script>
							var nameFile = <?php echo '"/' . $data['filename'] . '"';  ?>;
							function makeDELETErequest() {
								$.ajax({
									url: nameFile,
									type: 'DELETE',
									success: function (result) {
										document.getElementById('deleted').textContent = nameFile + ": successfully deleted";
										console.log(nameFile + ": successfully deleted")
									},
									error: function (result) {
										document.getElementById('deleted').textContent = "somethig goes wrong";
										console.log("somethig goes wrong")
									}
								});
							}
						</script>
						<div>
							<button class="btn btn--outline reveal-on-scroll is-revealing" onclick="makeDELETErequest()">
								Click here if you want to delete the file
							</button>
						</div>
					<?php else : ?>
						<p>You didn't enter a file name</p>
					<?php endif; ?>
                </div>
				<p id="deleted"></p>
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







<!-- <!DOCTYPE html>
<html lang="en">

<head>
	<meta charset="UTF-8">
	<meta name="viewport" content=
		"width=device-width, initial-scale=1.0">
	
	 Importing the jQuery
	<script src="https://cdnjs.cloudflare.com/ajax/libs/jquery/3.3.1/jquery.min.js"></script>
</head>

<script>
							function makeDELETErequest() {
								$.ajax({
									url: '/logo%20grande%20taille.jpg',
									type: 'DELETE',
									success: function (result) {
										console.log("deleted")
									}
								});
							}
						</script>


<body>
	<button onclick="makeDELETErequest()">
		Click for DELETE request
	</button>
</body>

</html> -->
