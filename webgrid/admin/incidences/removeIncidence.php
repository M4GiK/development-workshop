<?
	session_start();
	if($_GET["logout"] == "true")
	{
		session_unregister("admin");
		header("location:../../index.php");
		exit();
	}
	else
	{
		//session_register("admin");
		if(!isset($_SESSION["admin"]))
			header("location:../login.php");
	}	
	
	
?>	

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>Web GRID</title>
<link href="../../frontend/style-css/style_admin_usuario.css" rel="stylesheet" type="text/css" />
 <!--[if IE 6]>
        <link rel="stylesheet" type="text/css" href="style-css/style_ie6_admin_usuario.css" media="all" charset="utf-8" />
 <![endif]--> 
 <!--[if IE 7]>
        <link rel="stylesheet" type="text/css" href="style-css/style_ie7_admin_usuario.css" media="all" charset="utf-8" />
 <![endif]-->
 
<meta http-equiv="content-language" content="cas" />
</head>

<body>
<?
include "../menus.php";
?>
<div id="contenido">
	<div id="cabecera">
		<div id="logo"><? include "../header.htm"?></div>
	</div>
	<div id="central">
		<div id="primer_menu">
			<ul class="ul_primermenu">
				<? getFirstMenuGestion();?>
			</ul>
		</div>	
		
	<div id="pestanas"> 
		<ul class="ul_pestana">
			<? getTabsGestion("Incidencias",$num);?>
		</ul>
	</div>
	<div id="parte_central">
	<div id="menu_interior">
		<ul class="ul_segundomenu">
			<li class="li_segundomenu"><a href="index.php">Volver</a></li>
		</ul>
	</div>	 
<div id="result" align="center">
<h1 align="center">Editar incidencia<br/></h1>
<?
	include "../config.php";
	include "../../management/incidenceManager.php";

	$im = new incidenceManager();
	
	if($im->removeIncidence($_GET["incidence"]))
		echo "<p align=\"center\"><img src=\"../../frontend/img/icons/accept.png\"/>&nbsp;Se ha eliminado la incidencia con &eacute;xito!</p>";
	
	else
		echo "<p align=\"center\"><img src=\"../../frontend/img/icons/exclamation.png\"/>&nbsp;No se ha podido eliminar<p/>";
	
	
	
echo "<p align=\"center\"><img src=\"../../frontend/img/icono_amarill_square.gif\"/><a href=\"index.php\">&nbsp;volver</a></p>";
?>
</div>
<br />
<br />

	</div>
</div>
<div id="peu">&nbsp;</div>
</div>
</body>
</html>