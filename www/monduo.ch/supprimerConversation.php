<?php
require_once 'fonctions.php';

$conversationNO ='';
$conversationNO = $_POST['conversationDelete'];

if (!empty($conversationNO)) {
    queryMySQL("DELETE FROM conversations WHERE conversationNO = '$conversationNO'");
    queryMySQL("DELETE FROM messages WHERE conversationNO = '$conversationNO'");
    exit;
}
?>