<?php
	
	$nm = new newsManager();
	
	switch($_GET["lang"])
		{
			case "EN":
						echo "<h2>NEWS</h2>";
						break;
			case "ES":
						echo "<h2>NOTICIAS</h2>";
						break;
			
			case "CA":
						echo "<h2>NOT&Iacute;CIES</h2>";
						break;						
						
		}
	
	if(isset ($_GET["lang"])  )
	{	
		
		$lang = $_GET["lang"];
		
		$news = $nm->getNews(time());
		$n = count($news);
		
		if($n > 0)
			echo "<ul class=\"ul_noticias\">";
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
			
			echo "<li class=\"li_noticias\"><a href=\"news.php?id=$link\" class=\"enllac\"> $titular</a></li>";
		}
		
		if($n > 0)
		echo "</ul>";
	}	

?>