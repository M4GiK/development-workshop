<?
	session_start();
	//session_register("admin");
	if(!isset($_SESSION["admin"]))
	{
			header("location:../login.php");
			exit();
	}
?>			
<?
	include "../config.php";
	include "../../management/newsManager.php";
	
	$new = $_GET["new"];
	
	$newsm = new newsManager();
	
	if($newsm->removeNew($new))
		echo "Se ha borrado la noticia con exito!<br>";
	else
		echo "No se ha podido borrar la noticia<br>";	
		
			
																
	
	
	echo "<a href=\"index.php\">volver</a>";		
?>
