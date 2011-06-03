<?php


/**
 * Function to return admin setting form
 * @return array() containing the admin form
 */
 
function islandora_solr_custom_admin_settings() {

  $form = array();
  /* Disabled for now.
  $form['islandora_solr_custom_fields'] = array(
    '#type' => 'textarea',
    '#title' => t('Fields to output'),
    '#default_value' => variable_get('islandora_solr_custom_fields', 'dc.title'),
    '#description' => t('Fill out the solr fields you want to display in the search results. One field name per line.'),
  );
  */
  //dsm(_islandora_solr_custom_db_values());

  //dsm($form);
  
  
  
  
  
  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save and generate markup')
  );
  $form['buttons']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset to defaults')
  );

  if (!empty($_POST) && form_get_errors()) {
    drupal_set_message(t('The settings have not been saved because of the errors.'), 'error');
  }
  
  $form['div'] = array(
    '#type' => 'fieldset',
    '#title' => t('Div based markup'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['div']['islandora_solr_custom_output'] = array(
    '#type' => 'markup',
    '#value' => _islandora_solr_custom_markup_render(_islandora_solr_custom_solrconfig()),
  );
  
  $form['#submit'][] = 'islandora_solr_custom_settings_form_submit';
  //$form['#theme'] = 'system_settings_form';
  
  
  return ($form);
}


/**
 * Implementation of hook_form_submit()
 * @param array $form
 * @param array $form_state
 * @return null 
 */
 
function islandora_solr_custom_settings_form_submit($form, &$form_state) {

  dsm($form_state);
  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';

  // Exclude unnecessary elements.
  unset($form_state['values']['submit'], $form_state['values']['reset'], $form_state['values']['form_id'], $form_state['values']['op'], $form_state['values']['form_token'], $form_state['values']['form_build_id']);
  // dsm($form_state);
  foreach ($form_state['values'] as $key => $value) {
    if ($op == t('Reset to defaults')) {
      variable_del($key);
    }
    else {
      if (is_array($value) && isset($form_state['values']['array_filter'])) {
        $value = array_keys(array_filter($value));
      }
      variable_set($key, $value);
    }
  }
  if ($op == t('Reset to defaults')) {
    drupal_set_message(t('The configuration options have been reset to their default values.'));
  }
  else {
    drupal_set_message(t('The solr custom fields have been saved.'));
  }

  cache_clear_all();
  drupal_rebuild_theme_registry();
}



/**
 * Function that returns the DB input and saves new data if the table is empty.
 * It will also output the database values in the order of the values in solrconfig.xml.
 * @return islandora_solr_custom DB values
 * @TODO add a solution/notification if it can't connect to solrconfig.xml correctly
 */
 
function _islandora_solr_custom_db_values() {

// This selects all rows in the order of the database:
//  $sql = "SELECT * FROM {islandora_solr_custom}";
//	$res = db_query($sql);
//  while($db_array = db_fetch_array($res)) {
//    $item[] = $db_array;
//  }
  
  // retrieve all information from the DB table and output them in the order as defined in solrconfig.xml
  $item = array();
  $solrconfig = _islandora_solr_custom_solrconfig();
  foreach($solrconfig as $field) {
    $sql = "SELECT * FROM {islandora_solr_custom} WHERE field = '$field'";
    $res = db_query($sql);
    $item[] = db_fetch_array($res);
  
  }
  
  //dsm($item);
  
  
  
  
	// if the DB table is empty, then fill it with information fetched from solrconfig.xml
	// if it can't connect to solrconfig.xml, output a notification.
	if (!$item) {
	
	  $solrconfig = _islandora_solr_custom_solrconfig();
	  //dsm($solrconfig);
	  
	  foreach($solrconfig as $field) {
      db_query("INSERT INTO {islandora_solr_custom} (field, label, exclude_label, markup) VALUES('%s', '%s', %d, %d)", $field, $field, 0, 1);
      // I should try to use this, but it doesn't work yet.
      // drupal_write_record("islandora_solr_custom, &$field, $update = array());
    }
	  
	  // retrieve all information from the DB table and output them in the order as defined in solrconfig.xml
    $item = array();
    $solrconfig = _islandora_solr_custom_solrconfig();
    foreach($solrconfig as $field) {
      $sql = "SELECT * FROM {islandora_solr_custom} WHERE field = '$field'";
      $res = db_query($sql);
      $item[] = db_fetch_array($res);
    }
	 
	}
	
	
  return $item;
}



/**
 * Function that connects to solrconfig.xml
 * @return an array that contains all the solr fields that are set in solrconfig.xml in <str name="fl"> ... </str>
 */
 
function _islandora_solr_custom_solrconfig() {

  // xpath experiment

  $xmlDoc = new DOMDocument();
  $xmlFile = 'http://' . variable_get('islandora_solr_search_block_url', 'localhost:8080/solr') . '/admin/file/?file=solrconfig.xml';
  $xmlDoc->load($xmlFile, LIBXML_NOCDATA);
  $xpath = new DOMXPath($xmlDoc);

  // xPath query.
  // variable wich states which requestHandler we're dealing with.
  $requestHandlerVariable = variable_get('islandora_solr_search_block_request_handler', t('standard'));
  $query = '//requestHandler[@name="'. $requestHandlerVariable .'"]//str[@name="fl"]';

  $entries = $xpath->query($query);  
  
  $result = '';
  foreach ($entries as $entry) {
    $result .= "{$entry->nodeValue}";
  }
  
  // set error message if impossible to connect.
  if($result == '') {
    drupal_set_message( t('Failed to connect to @string. Please check your <a href="@link">solr settings</a>.' , array('@string' => $xmlFile, '@link' => 'admin/settings/islandora_solr_search')), 'error' );
    $result = FALSE;
  }
  
  // exploding $result into an array
  if($result != FALSE) $result = explode(',', $result);
  
  return $result;

}



