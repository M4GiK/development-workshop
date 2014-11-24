<?

/*
 * @class PresentationManager
 * @brief Manager de la capa presentación
 * @autor David Martos Expósito 
 *
 */
 include_once "management/contentManager.php";
 include_once "management/newsManager.php";
 include_once "management/eventManager.php";
 
 class viewManager
 {
 	//atributos
	
	var $language;
	var $section;
	var $subsection;
	var $month;	
	var $year;
	/*
		constructora
	*/	
	function viewManager($lang)
	{
		$this->detectLanguage();
		//$this->language = "ES";
		//echo "language:".$this->language;
		$this->section = 8;
		$this->subsection = "";
		$date = getdate(time());
		$this->month = $date["mon"];
		$this->year = $date["year"];
		
	}
	
	function detectLanguage()
	{
		/*print('Lenguage aceptado: ');
		print($_SERVER['HTTP_ACCEPT_LANGUAGE']);
		print('<br>');*/
		 
		$language = $_SERVER[HTTP_ACCEPT_LANGUAGE];
		 
		if(strstr($language,",")) 
		{
		 	$list1 = explode(",", $language);
			$i = 0;
			while ($i <= sizeof($list1)) 
			{
				$list2 = explode(";", $list1[$i]);
			 	$list3[$i] = $list2[0];
				$i++;
		   } 
		} 
		else 
		{
			 $list3 = explode(";", $language);
		}
 
 		//echo 'list3 = '; print_r ($list3);  echo '<br>';
		$match = false;
		
		$this->language = "EN";
		$i = 0;
		while($i < count($list3) -1 && $match == false)
		{
			$lang = $list3[$i];
			switch($lang)
			{
				case "en":
							$match = true;
							break;
				case "es":
							$this->language = "ES";
							$match = true;
							break;
				case "ca":
							$this->language = "CA";
							$match = true;
							break;
			}
			$i++;
		}

 
	}
	
	/*
		Carga del menu
	*/
	function loadMenu($ajax)
	{
		
		$_GET['lang'] = $this->language;
		$_GET['selected'] = $this->section;
		$_GET['ajax'] = $ajax; 
		include_once "frontend/menu.php";
	}
	
	function loadMenuByName($name)
	{
		$cms = new contentManager();
		$info = $cms->getSectionByName($name);
		$this->section = $info["id_seccion"];
		
		
		$_GET['lang'] = $this->language;
		$_GET['selected'] = $this->section;
		$_GET['ajax'] = $ajax; 
		include_once "frontend/menu.php";
	}
	
	// MODS
	/*
		carga mod pasado como parametro
	*/
	function loadBriefs()
	{
		$_GET['lang'] = $this->language;
		include_once "frontend/brief.php"; 
	}
	
	function loadNews($new)
	{
		$_GET['lang'] = $this->language;
		$_GET['id'] = $new;
		include_once "frontend/news.php";
	}
	
	function loadCalendar()
	{
		$_GET['mes'] = $this->month;
		$_GET['year'] = $this->year;
		include_once "frontend/calendar.php";
	}
	
	function loadEvents($filter,$day)
	{
		$_GET['day'] = $day;
		$_GET['month'] = $this->month;
		$_GET['year'] = $this->year;
		$_GET['filter'] = $filter;
		$_GET['lang'] = $this->language;
		include_once "frontend/events.php";
		
	}
	
	
	// INTERACTIVAS
	
	/*
		Carga las susbsecciones de la seccion pasada como parametro
	*/
	function loadSubsections($section,$ajax)
	{
		$_GET['selected'] = $this->subsection;
		$_GET['section'] = $section;
		$_GET['lang'] = $this->language; 
		$_GET['ajax'] = $ajax; 
		include_once "frontend/subsections.php";	
	}
	
	/*
		Carga el contenido de seccion-subseccion pasado como parametro
	*/
	function loadContent($section,$subsection,$ajax)
	{
		$this->section = $section;
		$this->subsection = $subsection;
		
		//echo "section:$section subsection:$subsection <br>";
		$cms = new contentManager();
		$content = $cms->getContent($section,$subsection,$this->language);
		if($content == "")
			$content = "&nbsp;";
			
		if($ajax)
			echo urlencode($content);
		else
			echo $content;
		
		
	}
	
	/*
		cambia el lenguaje de la aplicacion
	*/
	function changeLanguage($lang)
	{
		$this->language = $lang;
	}
	
	function setCurrentSection($section, $subsection)
	{
		$this->section = $section;
		$this->subsection = $subsection;
	}
	
	function setCurrentCalendar($month,$year)
	{
		$this->month = $month;
		$this->year = $year;
	}
	
	function loadCurrentContent($ajax)
	{
		$this->loadContent($this->section, $this->subsection,$ajax);
	}
	
	function loadCurrentSubsections($ajax)
	{
		$this->loadSubsections($this->section,$ajax);
	}
	
	/*function loadCurrentSubsectionsAndSection($ajax, $section)							//overloaded function
	{
		$this->loadSubsections($section, $ajax);
	}*/
	
	function getLanguage()
	{
		return $this->language;
	}
 }
 
?>
