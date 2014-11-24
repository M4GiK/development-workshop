<?
	session_start();
	//session_register("admin");
	
	if(!isset($_SESSION["admin"]))
		header("location:login.php");
	
	
	
?>	
<?
	include "configManager.php";
	include "../config.php";
	
	
	echo "<pre>";
	print_r($_POST);
	echo "</pre>";
	$cm  = new configManager();
	$cm->editDefaultParams($_POST);
	echo "</br></br><a href=\"index.php\">volver </a>";
	
?>