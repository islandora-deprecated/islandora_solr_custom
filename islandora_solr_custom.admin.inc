<?php

/**
 * Function to set debug options. It provides dummy search results and dummy solrconfig.xml field results.
 * @return change this to TRUE or FALSE whether you want to use sample data or actual solr data.
 */

function _islandora_solr_custom_debug() {

  $debug_mode = FALSE;

  return $debug_mode;
}


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
  
  $form['display_style'] = array(
    '#type' => 'fieldset',
    '#title' => t('Display style'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => TRUE, // this attribute is important to return the submitted values in a deeper nested arrays in $form_state['values'] . See http://drupal.org/node/751826
    '#description' => t('Configure the display style.'),
    '#weight' => -1,
  );
  $form['display_style']['display'] = array(
    '#type' => 'select',
    '#title' => t('Default display style'),
    '#default_value' => variable_get('islandora_solr_custom_display','div'),
    '#options' => array(
      'div' => t('Div'),
      'table' => t('Table'),
    ),
    '#description' => t('Set the display style of the solr search results.'),
    '#weight' => -10,
  );
  $form['display_style']['switch'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add display switch link?'),
    '#default_value' => variable_get('islandora_solr_custom_switch', 0),
    '#description' => t('Check if you want to include a link to switch between the two display styles.'),
  );
  $form['css'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include css?'),
    '#default_value' => variable_get('islandora_solr_custom_css', 1),
    '#description' => t('Check if you want to include some basic css styling. You can still overwrite css in the theme layer. Only uncheck if you really want to take full control over the css styling. This will not affect the table display.'),
  );

  
  
  
  $form['islandora_solr_custom_table'] = array(
    '#type' => 'fieldset',
    '#title' => t('Field settings'), // doesn't work with table render.
    '#collapsible' => FALSE, // doesn't work with table render.
    '#collapsed' => FALSE,
    '#tree' => TRUE, // this attribute is important to return the submitted values in a deeper nested arrays in $form_state['values'] . See http://drupal.org/node/751826
    '#description' => t('description'), // doesn't work with table render.
    '#weight' => -1,
  );
  
  $solrconfig = _islandora_solr_custom_db_values();
  foreach($solrconfig as $field) {
  
    //dsm($field);
    $fieldname = $field['field'];
    
    $form['islandora_solr_custom_table'][$fieldname]['field'] = array(
      '#type' => 'markup',
      '#value' => $field['field'],
      '#weight' => -1,
    );
    $form['islandora_solr_custom_table'][$fieldname]['hidden'] = array(
      '#type' => 'hidden',
      '#value' => $field['field'],  // to know which row in the DB to update with the new values on save.
      '#weight' => -1,
    );
    // if the row is just added.
    if (isset($field['new'])) {
    $form['islandora_solr_custom_table'][$fieldname]['new'] = array(
      '#type' => 'hidden',
      '#value' => $field['new'],  // to know if this row is added in the same POST request as the _submit request. We don't update the DB then. It would override the default settings with empty values.
      '#weight' => -1,
    );
    }
    $form['islandora_solr_custom_table'][$fieldname]['label'] = array(
      '#type' => 'textfield',
      '#default_value' => $field['label'],
      '#weight' => -1,
    );
    $form['islandora_solr_custom_table'][$fieldname]['exclude_label'] = array(
      '#type' => 'checkbox',
      '#default_value' => $field['exclude_label'],
      '#weight' => -1,
    );
    $form['islandora_solr_custom_table'][$fieldname]['markup'] = array(
      '#type' => 'checkbox',
      '#default_value' => $field['markup'],
      '#weight' => -1,
    );
  
  }
  
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
  
  $form['custom_markup'] = array(
    '#type' => 'fieldset',
    '#title' => t('Custom markup'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['custom_markup']['islandora_solr_custom_output'] = array(
    '#type' => 'markup',
    '#value' => _islandora_solr_custom_markup_render(_islandora_solr_custom_solrconfig()),
  );
  
  $form['#submit'][] = 'islandora_solr_custom_settings_form_submit';
  //$form['#validate'][] = 'islandora_solr_custom_settings_form_validate';
  $form['#theme'] = 'islandora_solr_custom_admin_settings';
  //$form['#theme'] = 'system_settings_form';
  
  return $form;
}

function islandora_solr_custom_admin_settings_validate($element, &$form_state) {
      
      //dsm($element);
      //dsm($form_state);
      
      // I should add some validation rules here.
      
      //if (empty($element['#value'])) {
      //     form_error($element['#name'], t('This field is required.'));
      //   }
}



/**
 * Implementation of hook_form_submit()
 * @param array $form
 * @param array $form_state
 * @return null 
 */
 
function islandora_solr_custom_settings_form_submit($form, &$form_state) {

  //dsm($form_state);
  
 //This selects all rows in the order of the database:
  $sql = "SELECT * FROM {islandora_solr_custom}";
	$res = db_query($sql);
  while($db_array = db_fetch_array($res)) {
    $item[] = $db_array;
  }
  //dsm($item);  
  
  // saving the table results
  // saving only the results of fields that are already in the DB to prevent saving empty values.
  $table_results = $form_state['values']['islandora_solr_custom_table'];
  foreach($table_results as $result) {
  
    $field = $result['hidden'];
   // $new = $result['new'];
  
    if (!isset($result['new'])) {

      db_query("UPDATE {islandora_solr_custom} SET label = '%s', exclude_label = %d, markup = %d WHERE field = '%s'", $result['label'], $result['exclude_label'], $result['markup'], $result['hidden']);
    // I should try to use this, but it doesn't work yet.
    // drupal_write_record("islandora_solr_custom, &$field, $update = array());
    }
  }
  
  // saving the style result
  variable_set('islandora_solr_custom_display', $form_state['values']['display_style']['display']);
  
  // saving the switch result
  variable_set('islandora_solr_custom_switch', $form_state['values']['display_style']['switch']);
  
  // saving the css result
  variable_set('islandora_solr_custom_css', $form_state['values']['css']);
  
  
  
  
  
  // I should check all that comes after this. And write a function to reset the values.
  
  
  
  if ($form_state['clicked_button']['#id'] != 'edit-submit') {
    $form_state['rebuild'] = TRUE;
    return;
  }
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
  
 
  
  // check if the DB table is empty or not
  $count = db_query("SELECT count(*) FROM {islandora_solr_custom}");
  $count_result = db_result($count);
  
  // retrieve all information from the DB table and output them in the order as defined in solrconfig.xml
  $item = array();
  $solrconfig = _islandora_solr_custom_solrconfig();
  foreach($solrconfig as $field) {
    $sql = "SELECT * FROM {islandora_solr_custom} WHERE field = '$field'";
    $res = db_query($sql);
    $row_result = db_fetch_array($res);
    
    // if the field is not in the DB, add it using default settings and fetch it again.
    if(!is_array($row_result)) {
      // add it to the DB
      db_query("INSERT INTO {islandora_solr_custom} (field, label, exclude_label, markup) VALUES('%s', '%s', %d, %d)", $field, $field, 0, 1);
      // and fetch it again
      $sql = "SELECT * FROM {islandora_solr_custom} WHERE field = '$field'";
      $res = db_query($sql);
      $row_result = db_fetch_array($res);
      $row_result['new'] = TRUE; // if the row just got added now, add a 'new' value. This will be put in a hidden field. This is needed so when saving table results while there are new fields added, the results will be saved, but no values would be set yet. This would override the default values set in this function with empty values.
    }

    $item[] = $row_result;
    
  }
   
  // clean up rows that are not in the $solrconfig results
  $sql = "SELECT * FROM {islandora_solr_custom}";
	$res = db_query($sql);
  while($db_array = db_fetch_array($res)) {
    if(!in_array($db_array['field'], $solrconfig)) {
    
      $field = $db_array['field'];
      $sql = "DELETE FROM {islandora_solr_custom} WHERE field = '$field'";
      db_query($sql);
      drupal_set_message( t("@field is not defined in solrconfig.xml and is removed.", array('@field' => $field)) );
    
    }
    //dsm($db_array);
  }
   
   
  
  //dsm($item);
	
  return $item;
}



/**
 * Function that connects to solrconfig.xml
 * @return an array that contains all the solr fields that are set in solrconfig.xml in <str name="fl"> ... </str>
 */
 
function _islandora_solr_custom_solrconfig() {

  // if $debug is set to TRUE, then it just makes a quick dummy result array instead of looking for solrconfig.xml.
  $debug = _islandora_solr_custom_debug();
  
  // setup a variable for debugging. I probably should remove this stuff later.
  if($debug == TRUE) {
  
    $result = array('dc.title', 'dc.foo1', 'dc.foo2', 'dc.foo3');
  
  } else {

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
  
  }
  
  return $result;

}

