<?

/*
 * @class PresentationManager
 * @brief Manager de la capa presentación
 * @autor David Martos Expósito 
 *
 */
 include_once "management/contentManager.php";
 include_once "management/newsManager.php";
 
 class viewManager
 {
 	//atributos
	
	var $language;
	var $section;
	var $subsection;
	/*
		constructora
	*/	
	function viewManager($lang)
	{
		$this->language = $lang;
		$this->section = 1;
		$this->subsection = "";
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
	
	function loadNews()
	{
		$_GET['lang'] = $this->language;
		include_once "frontend/news.php";
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
		if($ajax)
			echo urlencode($cms->getContent($section,$subsection,$this->language));
		else
			echo $cms->getContent($section,$subsection,$this->language);	
		
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
	
	function loadCurrentContent($ajax)
	{
		$this->loadContent($this->section, $this->subsection,$ajax);
	}
	
	function loadCurrentSubsections($ajax)
	{
		$this->loadSubsections($this->section,$ajax);
	}
 }
 
?>
