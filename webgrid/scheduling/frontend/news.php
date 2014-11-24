<?
	function showLink($new,$lang)
	{
		switch($lang)
		{
			case "ES":
						$titular = $new["titular_es"];
						break;
			case "EN":
						$titular = $new["titular_en"];
						break;
						
			case "CA": 
						$titular = $new["titular_ca"];
						break;
		}
		$fecha = date("d/m/Y",$new["fecha"]);
		$id = $new["id_noticia"];
		
		echo $seccion;
		echo "<p><a href=\"news.php?id=$id\">$titular ( $fecha )</a></p>";
	}
	function showNew($new,$lang)
	{
		
		switch($lang)
		{
			case "ES":
						$titular = $new["titular_es"];
						$cuerpo = $new["cuerpo_es"];
						$return = "ver todas las noticias";
						
						break;
			case "EN":
						$titular = $new["titular_en"];
						$cuerpo = $new["cuerpo_en"];
						$return = "show all news";
						
						break;
						
			case "CA": 
						$titular = $new["titular_ca"];
						$cuerpo = $new["cuerpo_ca"];
						$return = "veure totes les not&iacute;cies";
						
						break;
		}
		$fecha = date("d/m/Y",$new["fecha"]);
		echo "<h1>$titular</h1>";
		echo "<p>$cuerpo</p>";
		echo "<p>$fecha</p>";
		echo "<p><a href=\"news.php\">$return</a></p>"; 
						
	}
	
	$newsm = new newsManager();
	
	if($_GET["id"] == "")
	{
		$news = $newsm->getNews(time());
		
		switch($_GET["lang"])
		{
			case "EN":
						echo "<h1>News</h1>";
						break;
			case "ES":
						echo "<h1>Noticias</h1>";
						break;
			
			case "CA":
						echo "<h1>Not&iacute;cies</h1>";
						break;						
						
		}
		
		for($i = 0; $i < count($news); $i++)
		{
			showLink($news[$i],$_GET['lang']);
		}
		
	}
	else
	{
		$new = $newsm->getNew($_GET["id"]);
		showNew($new,$_GET['lang']);
	}
	
?>
