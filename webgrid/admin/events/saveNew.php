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

	$time = mktime($_POST["hour"], 0, 0, $_POST["month"], $_POST["day"], $_POST["year"]);
	echo date("G:i d/m/Y",$time);
	$info["titular_es"] = $_POST["titular_es"];
	$info["titular_en"] = $_POST["titular_en"];
	$info["titular_ca"] = $_POST["titular_ca"];
	$info["cuerpo_en"] = $_POST["cuerpo_en"];
	$info["cuerpo_es"] = $_POST["cuerpo_es"];
	$info["cuerpo_ca"] = $_POST["cuerpo_ca"];
	$info["fecha_exp"] = $time;
	$new = $_POST["new"];
	$action  = $_POST["action"];
	
	print_r($info);
	
	$newsm = new newsManager();
	
	switch($action)
	{
		case "edit":	
					if($newsm->editNew($info,$new))
						echo "Se ha actualizado con exito!<br>";
					else
					echo "No se ha podido actualizar<br>";	
	
					break;
					
		case "new":
							if($newsm->addNew($info))
								echo "Se ha a&ntilde;adido la noticia con exito!<br>";
							else
								echo "No se ha podido a&ntilde;adir la notica<br>";	
		
							break;
		
											
	}
	
	
	echo "<a href=\"index.php\">volver</a>";		
?>
