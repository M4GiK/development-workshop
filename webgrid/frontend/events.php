<?
	
	function getInterval($event)
	{
		$interval = true;
		$ciclic_day = false;
		$ciclic_month = false;
		$ciclic_year = false;
		
		if($event["dia"] == $event["dia_fin"] && $event["mes"] == $event["mes_fin"] && $event["year"] == $event["year_fin"])
			$interval = false;
		
		if($event["dia"] == 0)
			$ciclic_day =true; 
		if($event["mes"] == 0)
			$ciclic_month = true;	
		if($event["year"] == 0)
			$ciclic_year = true;
		
		if($ciclic_day == false)
			$str = $event["dia"];
		else
			$str = "todos los dias";
		
		if($ciclic_month == false)
			$str .= " / ".$event["mes"];
		else					
			$str .= " / todos los meses ";
		
		if($ciclic_year == false)
			$str .= " / ".$event["year"];
		else
			$str .= " / todos los a&ntilde;os";
			
		if($interval == true)
		{
			$str.=" - ".$event["dia_fin"]." / ".$event["mes_fin"]." / ".$event["year_fin"];		
		}
		
		return $str;	 		
	}
	
	function showLink($event,$lang)
	{
		$id = $event["id_evento"];
		$intervalo = getInterval($event);
		
		switch($lang)
		{
			case "ES":
						$titulo = $event["titulo_es"];
						break;
			case "EN":
						$titulo = $event["titulo_en"];
						break;
						
			case "CA": 
						$titulo = $event["titulo_ca"];
						break;
		}
		
		echo "<p><a href=\"events.php?id=$id\">$titulo - ($intervalo)</a></p>";						
	}
	
	function showEvent($event,$lang)
	{
		$intervalo = getInterval($event);
		
		switch($lang)
		{
			case "ES":
						$descripcion = $event["evento_es"];
						$titulo = $event["titulo_es"];
						$return = "ver eventos del mes actual";
						break;
			case "EN":
						$descripcion = $event["evento_en"];
						$titulo = $event["titulo_en"];
						$return = "show all events of actual month";
						break;
						
			case "CA": 
						$descripcion = $event["evento_ca"];
						$titulo = $event["titulo_ca"];
						$return = "veure tots els events del mes actual";
						break;
		}
		
		echo "<h1>$titulo</h1>";
		echo "<p><strong>Fecha:</strong> $intervalo</p>";
		echo "<p><strong>Descripcion:</strong> $descripcion</p>";
		echo "<p><strong>Lugar:</strong> ".$event["lugar"]."</p>";
		echo "<p><a href=\"events.php\">$return</a></p>";
						
	}
	
	$em = new eventManager();
	

	
	if($_GET["id"]!= "")
	{
		$event = $em->getEvent($_GET["id"]);
		showEvent($event,$_GET['lang']);
	}
	else
	{
		switch($_GET["lang"])
		{
			case "EN":			
						$title = "<h1>Events of ";
						break;
						
			case "CA":
						$title = "<h1>Events de ";
						break;
			case "ES":
						$title = "<h1>Eventos de ";
						break;	
		}
		
		switch($_GET["filter"])
		{
			case "day":
						$events = $em->getDayEvents($_GET["day"],$_GET["month"], $_GET["year"]);
						$title .= $_GET["day"]." / ".$_GET["month"]." / ". $_GET["year"] ."</h1>";
						break;
			case "month":
							$events = $em->getMonthEvents($_GET["month"], $_GET["year"]);
							$title .= $_GET["month"]." / ". $_GET["year"] ."</h1>";
							break;
			case "year":
							$events = $em->getYearEvents($_GET["year"]);
							
							break;
			case "":
					$events = $em->getEvents();
					$title = "<h1>Todos los eventos</h1>";
					break;											
		}
		
		echo $title;
		for($i = 0; $i < count($events); $i++)
		{
			showLink($events[$i],$_GET['lang']);
		}
	}			
	
?>
