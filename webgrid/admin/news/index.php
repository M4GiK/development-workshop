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
	include "../../management/newsManager.php";
	include "../../management/incidenceManager.php";
	include "../menus.php";
	
	function addRowNew($titular,$id,$class)
	{
		echo "
				<tr>
    				<td class=\"$class\">$titular</td>
    				<td class=\"$class\"><a href=\"manageNew.php?action=edit&new=$id\">modificar</a> <a href=\"removeNew.php?new=$id\">eliminar</a> </td>
				</tr>";
	}
	
	function getNewsInfo()
	{
		$newsm = new newsManager();
		$news = $newsm->getNews("");
		
		for($i = 0; $i < count($news); $i++)
		{
			if($i % 2 != 0)
				$class= "td_tabladatos_IMPAR"; 
			else	
				$class= "td_tabladatos_PAR"; 
			
			addRowNew($news[$i]["titular_es"], $news[$i]["id_noticia"],$class);
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
			<? getTabsGestion("Noticias",$num);?>
		</ul>
	</div>	<div id="parte_central">
	
<div id="result" align="center">
<h1 align="center">Noticias<br/></h1>
<table width="80%" cellpadding="0" cellspacing="0" class="tabladatos">
  <tr>
    <th class="th_tabladatos"><strong>Noticia</strong></td>
    <th class="th_tabladatos"><strong>Ajustes</strong></td>
  </tr>
 <?
 	getNewsInfo();
 ?>
  <tr>
    <td colspan="2" class="td_tabladatos_PAR">&nbsp;</td>
  </tr>
  <tr>
    <td colspan="2" class="td_tabladatos_PAR"><a href="manageNew.php?action=new"> A&ntilde;adir noticia</a></td>
  </tr>
  <tr class="tr_tabladatosfin">
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

<br />
</body>
</html>
