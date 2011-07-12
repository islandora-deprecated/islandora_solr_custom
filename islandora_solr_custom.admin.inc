<?php

/**
 * @file
 * Holds the admin functions.
 */


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
  



  // form: Field settings
  // create a new fieldset where we can put the rendered table in. Couldn't make that work with the actual fieldset of the table form items.  
  $form['islandora_solr_custom_table_output'] = array(
    '#type' => 'fieldset',
    '#title' => t('Field settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => FALSE, // not needed as this fieldset just outputs the rendered table.
    '#description' => t('Modify the field settings'),
    '#weight' => -5,
  );
  $form['islandora_solr_custom_table_output']['table_output_markup'] = array(
    '#type' => 'markup',
    '#value' => '',
  );
  
  // set the table form items
  $form['islandora_solr_custom_table'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE, // this attribute is important to return the submitted values in a deeper nested arrays in $form_state['values'] . See http://drupal.org/node/751826
  );
  
  $solrconfig = _islandora_solr_custom_db_values($check = TRUE); // get the solrconfig.xml and DB results. Also fix the differences between solrconfig.xml and the DB
  foreach ($solrconfig as $field) {
  
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
  
  
  // form: Display style
  $form['display_style'] = array(
    '#type' => 'fieldset',
    '#title' => t('Display style'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => TRUE, // this attribute is important to return the submitted values in a deeper nested arrays in $form_state['values'] . See http://drupal.org/node/751826
    '#description' => t('Configure the display style.'),
    '#weight' => -4,
  );
  $form['display_style']['display'] = array(
    '#type' => 'select',
    '#title' => t('Default display style'),
    '#default_value' => variable_get('islandora_solr_custom_display', 'div'),
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
  $form['display_style']['css'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include css?'),
    '#default_value' => variable_get('islandora_solr_custom_css', 1),
    '#description' => t('Check if you want to include some basic css styling. You can still overwrite css in the theme layer. Only uncheck if you really want to take full control over the css styling. This will not affect the table display.'),
  );


  // RSS settings
  $form['rss'] = array(
    '#type' => 'fieldset',
    '#title' => t('RSS'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => TRUE, // this attribute is important to return the submitted values in a deeper nested arrays in $form_state['values'] . See http://drupal.org/node/751826
    '#description' => '<p>' . t('You can set a default limit on items in your RSS feed by setting a default in the field below. Additionally, you can force a limit on rss results simply by adding a limit argument to the rss url. For example if you want to limit your items to 15 add this argument to your RSS url: <strong>&limit=15</strong>') . '</p>' .
                      '<p>' . t('The variables for the values of the RSS items and channel are set in theme functions. You can overwrite these functions in the theme layer and add an additional argument in the theme layer to set conditions in the function based on the value of the argument. For example: if you use <strong>&feed=myrss</strong> in the RSS url, then \'myrss\' will become available in the $feed variable in the theme functions, which you can use in a conditional statement to modify the variables.') . '</p>',
    '#weight' => -3,
  );
  $form['rss']['default_limit'] = array(
    '#type' => 'textfield',
    '#title' => t('Default RSS item limit'),
    '#default_value' => variable_get('islandora_solr_custom_rss_limit', 25),
    '#description' => t('The default limit of results to show in the RSS feed.'),
    '#size' => 2,
    '#maxlength' => 128,
    '#required' => FALSE,
  );
  
  
  
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
    '#description' =>   '<p>' . t('In your theme create a template file called <strong>islandora-solr-custom.tpl.php</strong> and paste in the code below. This will give you a nice start to customize your solr search results. You can then add wrapper elements, change labels, generate images from the repository, etc. Don\'t forget to rebuild the theme registry after doing this.') . '</p>' . 
                        '<p>' . t("Label overrides provided by the table above are applied, but label exclusion and showing markup when empty are not. Goal is for you to take full control over the output. It is also possible to override the theme functions of the table display and the switch link.") . '</p>' .
                        '<p>' . _islandora_solr_custom_template_overwrite() . '</p>',
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
      
      //dsm($element);
      if (!is_numeric($element['rss']['default_limit']['#value'])) {
        form_error(&$element['rss']['default_limit'], t('Default RSS limit must be a numeric value.'));
      }
      
      //drupal_set_message(form_get_errors());
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
  while ($db_array = db_fetch_array($res)) {
    $item[] = $db_array;
  }
  //dsm($item);  
  
  // saving the table results
  // saving only the results of fields that are already in the DB to prevent saving empty values.
  $table_results = $form_state['values']['islandora_solr_custom_table'];
  foreach ($table_results as $result) {
  
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
  variable_set('islandora_solr_custom_css', $form_state['values']['display_style']['css']);
  
  // saving the RSS limit result
  variable_set('islandora_solr_custom_rss_limit', $form_state['values']['rss']['default_limit']);
  
  
  
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
 * Function to render the custom markup
 * @return uses the given solr fields to render sample code to copy into islandora-solr-custom.tpl.php
 */
 
function _islandora_solr_custom_markup_render($markup) {

  $output = '';
  
  $output .= '<div class="form-item">';
  $output .= '<pre>';
  $output .= _islandora_solr_custom_markup_div($markup);
  $output .= '</pre>';
  $output .= '</div>'; 
  return $output;
}

/**
 * Function to render the div markup
 * @return div markup
 */
 
function _islandora_solr_custom_markup_div($markup) {

  // split $markup into arrays
  // $markup = nl2br($markup);
  //$markup = explode("\r\n", $markup);
  // $markup = str_replace("\r\n", '', $markup);
  
  // set $output variable
  $output = '';
  
  $output .= '<?php

/**
 * @file islandora-solr-custom.tpl.php
 * Islandora solr search results template
 *
 * Variables available:
 * - $variables: all array elements of $variables can be used as a variable. e.g. $base_url equals $variables[\'base_url\']
 * - $base_url: The base url of the current website. eg: http://example.com .
 * - $user: The user object.
 *
 * - $style: the style of the display (\'div\' or \'table\'). Set in admin page by default. Overridden by the query value: ?display=foo
 * - $results: the array containing the solr search results
 * - $table_rendered: If the display style is set to \'table\', this will contain the rendered table.
 *    For theme overriding, see: theme_islandora_solr_custom_table() 
 * - $switch_rendered: The rendered switch to toggle between display styles
 *    For theme overriding, see: theme_islandora_solr_custom_switch() 
 *
 */
?>

<?php print $switch_rendered; ?>

<?php if ($style == \'div\'): ?>

<ul class="islandora_solr_results">
<?php if ($results == \'\'): print \'<p>\' . t(\'Your search yielded no results\') . \'</p>\'; ?>
<?php else: ?>
<?php foreach ($results as $id => $result): ?>
  <li class="islandora_solr_result">
  ';
  
  foreach ($markup as $field) {
  
  $output .= '  <div class="solr-field <?php print $result[\'' . $field . '\'][\'class\']; ?>">  
      <div class="label"><label><?php print t($result[\'' . $field . '\'][\'label\']); ?></label></div>
      <div class="value"><?php print $result[\'' . $field . '\'][\'value\']; ?></div>
    </div>
  ';
  }
  
  $output .= '</li>
<?php endforeach; ?>
<?php endif; ?>
</ul>

<?php elseif ($style == \'table\'): ?>

  <?php print $table_rendered; ?>

<?php endif;';

  return check_plain($output);
}








/**
 * Function that returns the DB input and saves new data if the table is empty.
 * It will also output the database values in the order of the values in solrconfig.xml.
 * @param $check: FALSE means don't check for changes in the DB. TRUE means check for changes in the DB.
 * @return islandora_solr_custom DB values
 * @TODO add a solution/notification if it can't connect to solrconfig.xml correctly
 */
 
function _islandora_solr_custom_db_values($check = FALSE) {

  // set $item variable
  $item = array();
  
  // on admin page, get DB info and update (add/delete/position) according to solrconfig.xml
  if ($check == TRUE) {
  
    // retrieve all information from the DB table and output them in the order as defined in solrconfig.xml
    $item = array();
    $solrconfig = _islandora_solr_custom_solrconfig();
    $pos_count = 0;
    foreach ( $solrconfig as $field) {
      $res = db_query("SELECT * FROM {islandora_solr_custom} WHERE field = '%s'", $field);
      $row_result = db_fetch_array($res);
    
      // if the field is not in the DB, add it using default settings and fetch it again.
      if (!is_array($row_result)) {
        // add it to the DB
        db_query("INSERT INTO {islandora_solr_custom} (field, label, exclude_label, markup) VALUES('%s', '%s', %d, %d)", $field, $field, 0, 1);
        // and fetch it again
        $res = db_query("SELECT * FROM {islandora_solr_custom} WHERE field = '%s'", $field);
        $row_result = db_fetch_array($res);
        $row_result['new'] = TRUE; // if the row just got added now, add a 'new' value. This will be put in a hidden field. This is needed so when saving table results while there are new fields added, the results will be saved, but no values would be set yet. This would override the default values set in this function with empty values.
      }
    
      // update the order of the fields in the DB.
      db_query("UPDATE {islandora_solr_custom} SET position = %d WHERE field = '%s'", $pos_count, $field);
      $pos_count++;
    
      // save in array
      $item[] = $row_result;
    
    }
   
    // clean up rows that are not in the $solrconfig results
    $res = db_query("SELECT * FROM {islandora_solr_custom}");
    while ($db_array = db_fetch_array($res)) {
      if (!in_array($db_array['field'], $solrconfig)) {

        $field = $db_array['field'];
        db_query("DELETE FROM {islandora_solr_custom} WHERE field = '%s'", $field);
        drupal_set_message( t("@field is not defined in solrconfig.xml and is removed.", array('@field' => $field)) );
      }
    }
  }
  else {
    // just get the data from the DB on normal search requests.
    
    // This selects all rows in the order of the database:
	  $res = db_query("SELECT * FROM {islandora_solr_custom} ORDER BY position");
    while($db_array = db_fetch_array($res)) {
      $item[] = $db_array;
    }
  }
   
   
   
  // check if the DB table is empty or not
  $count = db_query("SELECT count(*) FROM {islandora_solr_custom}");
  $count_result = db_result($count);
  if ($count_result == 0 AND user_access("access administration pages")) {
    $message = t("There are no fields set to display. Please check the settings on the Islandora Solr Custom <a href ='!link'>configuration page</a>.", array('!link' => '/admin/settings/islandora_solr_search/custom'));
    drupal_set_message($message, 'alert');
  }
   
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
  if ($debug == TRUE) {
  
    $result = array('dc.title', 'dc.foo1', 'dc.foo2', 'dc.foo3');
  
  } 
  else {

  // xpath experiment
  $xml_doc = new DOMDocument();
 
  // check if url has http://
  $block_url = variable_get('islandora_solr_search_block_url', 'localhost:8080/solr');
  $pos = strpos($block_url, 'http://');
  if ($pos === FALSE) {
    $block_url = 'http://' . $block_url;
  }
  
  $xml_file = $block_url . '/admin/file/?file=solrconfig.xml';
  $xml_doc->load($xml_file, LIBXML_NOCDATA);
  $xpath = new DOMXPath($xml_doc);

  // xPath query.
  // variable wich states which requestHandler we're dealing with.
  $request_handler_variable = variable_get('islandora_solr_search_block_request_handler', t('standard'));
  $query = '//requestHandler[@name="' . $request_handler_variable . '"]//str[@name="fl"]';

  $entries = $xpath->query($query);
  
  $result = '';
  foreach ($entries as $entry) {
    $result .= "{$entry->nodeValue}";
  }
  
  // set error message if impossible to connect.
  if ($result == '') {
    drupal_set_message( t('Failed to connect to @string. Please check your <a href="@link">solr settings</a>.' , array('@string' => $xml_file, '@link' => 'admin/settings/islandora_solr_search')), 'error' );
    $result = FALSE;
  }
  
  // exploding $result into an array
  if ($result != FALSE) $result = explode(',', $result);
  
  }
  
  return $result;

}

/**
 *  Function that looks for all template files that overwrite the module's
 *  islandora-solr-custom.tpl.php.
 *  @return if results are found, returns a table with theme name and path.
 */
 
function _islandora_solr_custom_template_overwrite() {

    drupal_rebuild_theme_registry();
    $themes = list_themes($refresh = FALSE);
    $templates = array('islandora_solr_custom');
    $overwritten = array();
    foreach ($themes as $theme) {
      $path = drupal_get_path('theme', $theme->name);
      $result = drupal_find_theme_templates($templates, '.tpl.php', $path);
      if (!empty($result)) {
        $overwritten[] = array(
          'name' => $theme->name,
          'path' => $result['islandora_solr_custom']['path'],
        );
      }
    }
    
    $message = '';
    if (empty($overwritten)) {
      $message .= t('islandora-solr-custom.tpl.php has not been overwritten yet.');
    }
    else {
      $message .= t('islandora-solr-custom.tpl.php has been overwritten in the following location(s):');
      foreach ($overwritten as $theme) {
        $row['name'] = $theme['name'];
        $row['path'] = $theme['path'] . '/islandora-solr-custom.tpl.php';
        $rows[] = $row;
      }
            
      $header = array(t('Theme'), t('Path'));
      $message .= theme_table($header, $rows);
    }

  return $message;
}

