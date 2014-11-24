<?	
	include "admin/config.php";
	include "frontend/viewManager.php";
	session_start();
	//session_register('presentation');
	if(!isset($_SESSION['presentation']))
		$_SESSION['presentation'] = new viewManager("ES");
	
	if(isset($_GET["section"]))
	{
		$_SESSION['presentation']->setCurrentSection($_GET["section"],$_GET["subsection"]);
	}
	if(isset($_GET["lang"]))
	{
		$_SESSION['presentation']->changeLanguage($_GET["lang"]);
	}	 
	
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>

<script src="ajax.js" type="text/javascript"> </script>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1" />
<title>SimWEB</title>
<link href="frontend/style/style.css" rel="stylesheet" type="text/css" />


</head>

<body>
<div id="contenedor">
<div id="idioma">
<div id="logo"></div>
	<a  href="news.php?lang=ES">castellano</a>
	<a href="news.php?lang=CA" >catalan</a>
	<a  href="news.php?lang=EN">ingles</a> 
    <br />
    <br />
</div>
<div id="menu">
<? 
	$_SESSION['presentation']->loadMenuByName("News");
?>
</div>

<div id="central">
<div id="subseccion">
<? 
	$_SESSION['presentation']->loadCurrentSubsections(false); 
?>
</div>
<div id="contenido">
<?
	$_SESSION['presentation']->loadCurrentContent(false);
?>
</div>
<div id="news">
<?
	$_SESSION['presentation']->loadBriefs();
?>
</div>

</div>
<div id="footer">
<br />
<br />
<p>
    <a href="http://validator.w3.org/check?uri=referer"><img
        style="border:0;width:88px;height:31px"
		src="http://www.w3.org/Icons/valid-xhtml10"
        alt="Valid XHTML 1.0 Strict" /></a>
	<a href="http://jigsaw.w3.org/css-validator/">
  <img style="border:0;width:88px;height:31px"
       src="http://jigsaw.w3.org/css-validator/images/vcss" 
       alt="Valid CSS!" />
 </a>	
    <a href="http://www.fib.upc.edu"><img src="frontend/img/fib2.png" alt="enlace fib" width="79" height="30" border="0" /></a> <a href="http://www.upc.edu"><img src="frontend/img/upc.jpg" alt="enlace UPC" width="40" height="40" border="0" /></a>
 

	</p>
</div>  
</div>

</body>
</html>
