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

echo 'Hello ';
echo $data['firstname'];
echo ' ';
echo $data['lastname'];
echo '<br>';
echo 'Your story:';
echo '<br>';
echo $data['story'];

?>