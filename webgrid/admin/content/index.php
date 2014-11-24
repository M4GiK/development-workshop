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
<body>
<?
	include "../config.php";
	include "../../management/contentManager.php";
	include "../../management/incidenceManager.php";
	include "../menus.php";
	
	if(isset($_GET["action"]))
	{
		$cms =  new contentManager();
		switch($_GET["action"])
		{
			case "moveSection":
								if($_GET["way"] == "up")
									$cms->moveSectionUpward($_GET["section"]);
								else
									$cms->moveSectionDownward($_GET["section"]);
								break;			
								
			case "moveSubsection":
									if($_GET["way"] == "up")
										$cms->moveSubsectionUpward($_GET["section"],$_GET["subsection"]);
									else
										$cms->moveSubsectionDownward($_GET["section"],$_GET["subsection"]);
									break;			
		}
	}	
?>
<?
	
	function getTableSubsection($subsections, $section)
	{
		$html = "<table width=\"100%\" cellspacing=\"0\" cellpadding=\"0\">";
      	
		for($i = 0; $i < count($subsections); $i++)
		{	  
	  		if($i % 2 != 0)
				$class= "td_tabladatos_IMPAR"; 
			else	
				$class= "td_tabladatos_PAR"; 
				
			$subsection = $subsections[$i]["titulo_es"];
			$id_subsection = $subsections[$i]["id_subseccion"];
			$html.="
					<tr>
					<td class=\"$class\">$subsection</td>
					<td class=\"$class\"><a href=\"manageSection.php?action=edit&section=$section&subsection=$id_subsection\">modificar</a> <a href=\"removeSection.php?action=removeSubsection&section=$section&subsection=$id_subsection\">eliminar</a>
					<a href=\"index.php?action=moveSubsection&way=up&section=$section&subsection=$id_subsection\">subir</a>
					<a href=\"index.php?action=moveSubsection&way=down&section=$section&subsection=$id_subsection\">bajar</a> </td>
				  	</tr>";
		}			
		 
		 $html.="
		 			<tr>
					<td colspan=\"2\" class=\"td_tabladatos_PAR\">&nbsp;</td>
		 		    </tr>
		  			<tr>
					<td colspan=\"2\" class=\"td_tabladatos_PAR\"><a href=\"manageSection.php?action=newSubsection&section=$section\">A&ntilde;adir subseccion </a></td>
		  </tr>
		</table>";
		
		return $html;
	}
	
	function addRowSection($id_section,$section, $subsections,$class)
	{
		echo "
				<tr>
    				<td class=\"$class\">$section</td>
    				<td class=\"$class\"><a href=\"manageSection.php?action=edit&section=$id_section\">modificar</a> <a href=\"removeSection.php?action=removeSection&section=$id_section\">eliminar</a>  <a href=\"index.php?action=moveSection&way=up&section=$id_section\">subir</a>
					<a href=\"index.php?action=moveSection&way=down&section=$id_section\">bajar</a>	</td>
    				<td align=\"center\">".getTableSubsection($subsections,$id_section)."</td>
				</tr>";
	}
	
	function getSectionsInfo()
	{
		$cms = new contentManager();
		$sections = $cms->getSections("ES");
		for($i = 0; $i < count($sections); $i++)
		{
			if($i % 2 != 0)
				$class= "td_tabladatos_IMPAR"; 
			else	
				$class= "td_tabladatos_PAR"; 
				
			$subsections = $cms->getSubsections($sections[$i]["id_seccion"],"ES");
			addRowSection($sections[$i]["id_seccion"],$sections[$i]["titulo_es"], $subsections,$class);
		}
	}
	$im = new incidenceManager();
	$num = $im->getNumberWaitingIncidences();
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
			<? getTabsGestion("Contenido",$num);?>
		</ul>
	</div>	<div id="parte_central">
<div id="result" align="center">

<h1 align="center">Contenido<br/></h1>
<table width="80%" cellpadding="0" cellspacing="0" class="tabladatos">
  <tr>
    <th class="th_tabladatos" scope="col"><strong>Seccion</strong></td>
    <th class="th_tabladatos" scope="col"><strong>Ajustes</strong></td>
    <th class="th_tabladatos" scope="col"><strong>Subsecciones</strong></td>
  </tr>
 <?
 	getSectionsInfo();
 ?>
  <tr>
    <td colspan="3" class="td_tabladatos_PAR">&nbsp;</td>
  </tr>
  <tr>
    <td colspan="3" class="td_tabladatos_PAR"><a href="manageSection.php?action=newSection">A&ntilde;adir Seccion </a></td>
  </tr>
     <tr class="tr_tabladatosfin">
      <td></td>
      <td></td>
      <td></td>
      </tr>
</table>
</div>
<br />
<br />

	</div>
</div>
<div id="peu">&nbsp;</div>
</div>
</body>
</html>
