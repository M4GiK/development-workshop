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
?><?
	include "../config.php";
	include "../../management/newsManager.php";
	include "../../management/incidenceManager.php";
		
	include "../menus.php";
	function showErrors($errors)
	{
		for($i = 0; $i < count($errors); $i++)
		{
			echo "<p align=\"left\"><img src=\"../../frontend/img/icons/exclamation.png\"/>&nbsp;".$errors[$i]."<p/>";
		}	
	}
	
	if($_POST["Submit"] !="")
	{
		$info = $_POST;
		$errors = array();
		
		if(!checkdate($_POST["month"],$_POST["day"],$_POST["year"]))
			$errors[] = "Fecha expiraci&oacute;n incorrecta";
		
		else
		{
			$time = mktime($_POST["hour"], 0, 0, $_POST["month"], $_POST["day"], $_POST["year"]);
			$info["fecha_exp"] = $time;
			$new = $_POST["new"];
			$action  = $_POST["action"];
			
			$newsm = new newsManager();
			
			switch($action)
			{
				case "edit":	
							if($newsm->editNew($info,$new))
								header("location:messageNew.php?message=edit");
							else
								$errors[] = "No se ha podido modificar";	
			
							break;
							
				case "new":
							if($newsm->addNew($info))
									header("location:messageNew.php?message=edit");
							else
								$errors[] = "No se ha podido a&ntilde;adir la noticia";	
				
							break;						
			}
		}	
	}
	else
	{
			
		$action = $_GET["action"];
		$new = $_GET["new"];
	
		
		$newsm = new newsManager();
		switch($action)
		{
			case "edit":
					//echo "section:$section subsection:$subsection<br/>";
						$info = $newsm->getNew($new);
					//print_r($info);
					
						break;
		}
	}	
	
	$im = new incidenceManager();
	$num = $im->getNumberWaitingIncidences();
	
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
	</div>	
	<div id="parte_central">
	<div id="menu_interior">
		<ul class="ul_segundomenu">
			<li class="li_segundomenu"><a href="index.php">Volver</a></li>
		</ul>
	</div>	 
<div id="result" align="center">
<h1 align="center">Editar noticias<br/></h1>
<div id="error"><? showErrors($errors);?></div>
<form action="manageNew.php" method="post" name="form1" id="form1">
  <table width="80%" cellpadding="0" cellspacing="0" class="tabladatos">
    <tr>
      <th class="th_tabladatos" scope="row"><strong>titular_es</strong></td>
      <td class="td_tabladatos_PAR"><input type="text" name="titular_es" value="<? echo $info["titular_es"];?>" /> 
      <br /></td>
    </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>titular_en</strong></td>
      <td class="td_tabladatos_PAR"><input type="text" name="titular_en" value="<? echo $info["titular_en"]; ?>" />
      <br /></td>
    </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>titular_ca</strong></td>
      <td class="td_tabladatos_PAR"><input type="text" name="titular_ca" value="<? echo $info["titular_ca"];?>"/>
          <br /></td>
    </tr>
	 <tr>
      <th class="th_tabladatos" scope="row"><strong>fecha_exp </strong></td>
      <td class="td_tabladatos_PAR">Dia: 
        <select name="day" size="1" id="day">
        <?
		   		$daysel = date("j",$info["fecha_exp"]);
				for($i = 1; $i <= 31; $i++)
				{
					if ($i != $daysel)
						echo "<option value=\"$i\">$i</option>";
					else
						echo "<option value=\"$i\" selected=\"selected\">$i</option>";
				}
		   ?>
        </select>
       Mes: 
       <select name="month" id="month">
         <?
		   		$monthsel = date("n",$info["fecha_exp"]);
				for($i = 1; $i <= 12; $i++)
				{
					if ($i != $monthsel)
						echo "<option value=\"$i\">$i</option>";
					else
						echo "<option value=\"$i\" selected=\"selected\">$i</option>";
				}
		   ?>
       </select>
        A&ntilde;o: 
        <input name="year" type="text" id="year" size="4" maxlength="4" value="<? echo date("Y",$info["fecha_exp"]);?>" />
         Hora: 
         <select name="hour" id="hour">
           <?
		   		$hoursel = date("H",$info["fecha_exp"]);
				for($i = 0; $i < 24; $i++)
				{
					if ($i != $hoursel)
						echo "<option value=\"$i\">$i</option>";
					else
						echo "<option value=\"$i\" selected=\"selected\">$i</option>";
				}
		   ?>
        </select>
         <br />
       <br /></td>
	 </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>cuerpo_es</strong></td>
      <td class="td_tabladatos_PAR"><textarea name="cuerpo_es" cols="80" rows="10"><? echo $info["cuerpo_es"];?></textarea>
      <br /></td>
    </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>cuerpo_en</strong></td>
      <td class="td_tabladatos_PAR"><textarea name="cuerpo_en" cols="80" rows="10"><? echo $info["cuerpo_en"];?></textarea>
      <br /></td>
    </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>cuerpo_ca</strong></td>
      <td class="td_tabladatos_PAR"><textarea name="cuerpo_ca" cols="80" rows="10"><? echo $info["cuerpo_ca"];?></textarea>
      <br /></td>
    </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>fecha </strong></td>
      <td class="td_tabladatos_PAR"><br />
      <? echo $info["fecha"];?>
	  <input type="hidden" name="new" value="<? echo $new;?>"/>
	  <input type="hidden" name="action" value="<? echo $action;?>"/>
	  </td>
    </tr>
     <tr>
      <td colspan="2" class="td_tabladatos_PAR"><input type="submit" name="Submit" value="guardar" /></td>
    </tr>
  </table>

</form></div>
<br />
<br />

	</div>
</div>
<div id="peu">&nbsp;</div>
</div>

<br />
</body>
</html>

