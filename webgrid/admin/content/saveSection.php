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
	include "../config.php";
	include "../../management/incidenceManager.php";
	include "../menus.php";
	
	$im = new incidenceManager();
	$num = $im->getNumberWaitingIncidences();
	
?>
<<div id="contenido">
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
			<? getTabsGestion("Contenido",$num);?>
		</ul>
	</div>
	<div id="parte_central">
	<div id="menu_interior">
		<ul class="ul_segundomenu">
			<li class="li_segundomenu"><a href="index.php">Volver</a></li>
		</ul>
	</div>	 
<div id="result" align="center">
<h1 align="center">Editar contenido<br/></h1>
<?
	include "../config.php";
	include "../../management/contentManager.php";

	$text = $_POST['texto_es'];	

	$info["titulo_es"] = $_POST["titulo_es"];
	$info["titulo_en"] = $_POST["titulo_en"];
	$info["titulo_ca"] = $_POST["titulo_ca"];
	$info["texto_es"] = $text;
	$info["texto_en"] = $_POST["texto_en"];
	$info["texto_ca"] = $_POST["texto_ca"];
	$info["link_externo"] = $_POST["link_externo"];
	$info["link"] = $_POST["link"];
	$section = $_POST["section"];
	$subsection = $_POST["subsection"];
	$action = $_POST["action"];
	
	$cms = new contentManager();
	
	switch($action)
	{
		case "edit":	
					if($cms->updateSection($section,$subsection, $info))
						echo "<p align=\"center\"><img src=\"../../frontend/img/icons/accept.png\"/>&nbsp;Se ha modificado la secci&oacute;n con &eacute;xito!</p>";
					else
					echo "<p align=\"center\"><img src=\"../../frontend/img/icons/exclamation.png\"/>&nbsp;No se ha podido modificar<p/>";
	
					break;
					
		case "newSection":
							if($cms->addSection($info))
							echo "<p align=\"center\"><img src=\"../../frontend/img/icons/accept.png\"/>&nbsp;Se ha a&ntilde;adido la seccion con &eacute;xito!</p>";
							else
								echo "<p align=\"center\"><img src=\"../../frontend/img/icons/exclamation.png\"/>&nbsp;No se ha podido a&ntilde;adir la secci&oacute;<p/>";
	
		
							break;
		
		case "newSubsection":
								if($cms->addSubsection($section,$info))
									echo "<p align=\"center\"><img src=\"../../frontend/img/icons/accept.png\"/>&nbsp;Se ha a&ntilde;adido la subsecci&oacute;n con &eacute;xito!</p>";
								else
								echo "<p align=\"center\"><img src=\"../../frontend/img/icons/exclamation.png\"/>&nbsp;No se ha podido a&ntilde;adir la subsecci&oacute;<p/>";
	
			
								break;
																
	}
	
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