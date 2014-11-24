<?php
	
	$nm = new newsManager();
	
	echo "<h3>Noticias</h3>";
	if(isset ($_GET["lang"])  )
	{	
		
		$lang = $_GET["lang"];
		
		$news = $nm->getNews();
		$n = count($news);
		
		if($n > 0)
			echo "<ul>";
		for($i = 0; $i < $n; $i++)
		{
			$link = $news[$i]["id_noticia"];
			switch($lang)
			{
				case "ES":
							$titular = $news[$i]["titular_es"];
							break;	
				
				case "EN":	$titular = $news[$i]["titular_en"];
							break;
				
				case "CA":	
							$titular = $news[$i]["titular_ca"];
							break;
			}
			
			echo "<li><a href=\"news.php?id=$link\"> $titular</a></li>";
		}
		
		if($n > 0)
		echo "</ul>";
	}	

?>