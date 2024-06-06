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
?>
