<?	
	include "admin/config.php";
	include "frontend/viewManager.php";
	session_start();
	//session_register('presentation');
	if(!isset($_SESSION['presentation']))
		$_SESSION['presentation'] = new viewManager("ES");
	
	if(isset($_GET["lang"]))
	{
		$_SESSION['presentation']->changeLanguage($_GET["lang"]);
	}
	if(isset($_GET["subsection"]))
	{
		$_SESSION['presentation']->setCurrentSection($_GET["section"],$_GET["subsection"]);
	}	 
	
	if(isset($_GET["month"]))
	{
		$_SESSION['presentation']->setCurrentCalendar($_GET["month"],$_GET["year"]);
	}
	
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1" />
<title>Web GRID</title>
<link href="frontend/style-css/style.css" rel="stylesheet" type="text/css" />
 <!--[if IE 6]>
        <link rel="stylesheet" type="text/css" href="frontend/style-css/style_ie6.css" media="all" charset="utf-8" />
 <![endif]--> 
 <!--[if IE 7]>
        <link rel="stylesheet" type="text/css" href="frontend/style-css/style_ie7.css" media="all" charset="utf-8" />
 <![endif]-->
 
<meta http-equiv="content-language" content="cas" />
</head>
<body>
<?
	function getLanguageMenu()
	{
		switch($_SESSION['presentation']->getLanguage())
		{
			case "ES":
						echo "<li class=\"li_idioma\"><span class=\"link_idioma_sel\">Castellano</span></li>
							<li class=\"li_idioma\"><a href=\"news.php?lang=CA&amp;id=".$_GET["id"]."\" class=\"link_idioma\">Catal&agrave;</a></li>
							<li class=\"li_idioma\"><a href=\"news.php?lang=EN&amp;id=".$_GET["id"]."\" class=\"link_idioma\">English</a></li>";
							break;

			case "EN":
						echo "	<li class=\"li_idioma\"><a href=\"news.php?lang=ES&amp;id=".$_GET["id"]."\" class=\"link_idioma\">Castellano</a></li>
							<li class=\"li_idioma\"><a href=\"news.php?lang=CA&amp;id=".$_GET["id"]."\" class=\"link_idioma\">Catal&agrave;</a></li>
							<li class=\"li_idioma\"><span class=\"link_idioma_sel\">English</span></li>";
							break;

			case "CA":
						echo "<li class=\"li_idioma\"><a href=\"news.php?lang=ES&amp;id=".$_GET["id"]."\" class=\"link_idioma\">Castellano</a></li>
							<li class=\"li_idioma\"><span class=\"link_idioma_sel\">Catal&agrave;</span></li>
								<li class=\"li_idioma\"><a href=\"news.php?lang=EN&amp;id=".$_GET["id"]."\" class=\"link_idioma\">English</a></li>";
							break;
		}	
	}

?>
<div id="contenido">
<div id="cabecera"><? include "header.htm";?></div>
<div id="idioma">
<ul class="ul_idioma">
<? getLanguageMenu();?>
</ul>
</div>
<div id="menu">
<? 
	$_SESSION['presentation']->loadMenuByName("News");
?>
</div>
<div id="central">
<div id="submenu"><p class="separador"> </p>
<? 
	$_SESSION['presentation']->loadCurrentSubsections(false); 
?>
<p class="separador"> </p></div>
<div id="contenido_central">
<?
	$_SESSION['presentation']->loadNews($_GET["id"]);
?>
</div>
<div id="lateral_derecho">
<div id="noticias">
<?
		$_SESSION['presentation']->loadBriefs();?>
</div>
<div id="calendario">
<?
		$_SESSION['presentation']->loadCalendar();
	?>
</div>

</div>
<div id="pie"><? include "footer.htm";?></div>
</div>
</div>
</body>
</html>
