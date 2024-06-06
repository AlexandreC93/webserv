<?php
// Vérification si la méthode de requête est POST
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    // Récupération des nombres depuis le formulaire
    $number1 = $_POST["number1"];
    $number2 = $_POST["number2"];

    // Vérification si les nombres sont des entiers
    if (is_numeric($number1) && is_numeric($number2)) {
        // Calcul de la multiplication
        $result = $number1 * $number2;
        // Affichage du résultat
        echo "Résultat de la multiplication : $result";
    } else {
        // Si les nombres ne sont pas valides, affichage d'un message d'erreur
        echo "Les nombres saisis ne sont pas valides.";
    }
}

echo "<!DOCTYPE html>

<html lang=\"en\">\n
    <head>\n
    <meta charset=\"UTF-8\">\n
    <meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n
    <title>Basic Calculator</title>\n
</head>\n
<body>";
echo "<h1>Result :</h1>";
echo "<p>$a x $b = $result</p>";
echo "</body>\n
</html>\n";
?>