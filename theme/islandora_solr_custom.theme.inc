<?php

/**
 * @file
 * This file holds all of islandora_solr_custom's theme functions
 */
 
 
/**
 * Theme function for theming the admin table
 * @return rendered table
 */
 
function theme_islandora_solr_custom_admin_settings($form) {

  $rows = array();
  foreach ($form['islandora_solr_custom_table'] as $key) {
    $row = array();
    // check if $key is really the array we need. we shouldn't select the #parent array for example.
    if (isset($key['field']['#type'])) {
 
      $row[] = array('data' => drupal_render($key['field']));
      $row[] = array('data' => drupal_render($key['label']));
      $row[] = array('data' => drupal_render($key['exclude_label']));
      $row[] = array('data' => drupal_render($key['markup']));
 
      $rows[] = $row;
    }
  }
  $form['islandora_solr_custom_table']['#access'] = FALSE; // don't unset $form['anything'] , use $form['anything']['#access'] = FALSE; instead!
 
  // Individual table headers.
  $header = array();
  $header[] = t('solr field');
  $header[] = t('label');
  $header[] = t('exclude label?');
  $header[] = t('show markup when empty?');
  
  // put the rendered table in another fieldset.
  $form['islandora_solr_custom_table_output']['table_output_markup']['#value'] = theme('table', $header, $rows);
  
  $output = '';
  $output .= drupal_render($form);
  
  return $output;
}
 
 
 
 
 /**
  * Theme function for theming the solr search results
  *
  * @param $results
  * The raw search results
  *
  * @return variables to be used in the template file.
  */
  
function islandora_solr_custom_preprocess_islandora_solr_custom(&$variables) {
  // base url
  global $base_url;
  $variables['base_url'] = $base_url;
  // include solr field order generated from solrconfig.xml
  $variables['field_order'] = _islandora_solr_custom_solrconfig(); // not sure if this is really needed, but I'll keep it in for now.
  // this variable might be more useful as the db values are parsed in the solrconfig order using the results of _islandora_solr_custom_solrconfig().
  // get the solrconfig.xml and DB results. Don't check or fix any differences between solrconfig.xml and the DB
  $variables['db_values'] = _islandora_solr_custom_db_values($check = FALSE);
  $db_values = $variables['db_values'];
  $variables['display'] = variable_get('islandora_solr_custom_display', 'div');
  $variables['display_url'] = (isset($_GET['display']) ? $_GET['display'] : NULL);
  $variables['table_rendered'] = ''; // set the variable. If the table style is selected, this variable will be populated with a rendered table.
  $variables['switch'] = variable_get('islandora_solr_custom_switch', 0);
  $variables['switch_rendered'] = ''; // set the variable. If the display switch is enabled, this variable will be populated with a rendered link.
  $variables['css'] = variable_get('islandora_solr_custom_css', 1);

  // set $variables['style'];
  $variables['style'] = '';
  if ($variables['display_url'] != NULL) {
    if ($variables['display_url'] == 'div') {
      $variables['style'] = 'div';
    }
    elseif ($variables['display_url'] == 'table') {
      $variables['style'] = 'table';
    }
    else {
    $variables['style'] = $variables['display'];
    }
  }
  else {
    $variables['style'] = $variables['display'];
  }
  
  // include css or not
  $css = variable_get('islandora_solr_custom_css', 1);
  if ($css == 1) {
    drupal_add_css(drupal_get_path('module', 'islandora_solr_custom') .'/css/islandora_solr_custom_tpl.css');
  }
  
  
  // go over each search result object
  foreach ($variables['results_raw']->response->docs as $doc) {
  
    $resultsArray = array();
    // go over each field within a search result object
    foreach ($db_values as $db_value) {
      
      $field = $db_value['field'];
      
      if (isset($doc->$db_value['field'])) {
        $value = $doc->$db_value['field'];
      
        if (is_array($value)) {
          $value = implode(", ", $value);
        }
          $resultsArray[$field]['value'] = $value;
      }
      else {
        $resultsArray[$field]['value'] = '';
      }

      // set the fields
      $resultsArray[$field]['label'] = $db_value['label'];
      $resultsArray[$field]['class'] = strtolower( preg_replace('/[^A-Za-z0-9]/', '-', $db_value['field']) ); // turn the field name in a string that is appropriate to use as a class name.
      $resultsArray[$field]['exclude_label'] = $db_value['exclude_label'];
      $resultsArray[$field]['markup'] = $db_value['markup'];
      
    }

  $variables['results'][] = $resultsArray;
  }
  
  
  // if 'display' is set to table, render the table.
  if ($variables['style'] == 'table') {
    if ($variables['results'] != '') {
    
      $db_values = $variables['db_values'];
      $results = $variables['results'];
    
      $output = theme('islandora_solr_custom_table', $db_values, $results );
    }
    else {
      $output = '<p>' . t('Your search yielded no results') . '</p>';
    }
    $variables['table_rendered'] = $output;
  }
  
  // if display switch is set, render the link
  if ($variables['switch'] == 1  AND $variables['results'] != '') {

    $display = ''; // set $display
    
      $db_display = $variables['display'];
      $switch_style = ''; // this is will be the oposite display style than then one enabled.
      if ($db_display == 'table') {
        $switch_style = 'div';
      }
      elseif ($db_display == 'div') {
        $switch_style = 'table';
      }

    

    if (isset($_GET['display'])) {
      if ($_GET['display'] == 'table') {
        $display = 'div';
      }
      elseif ($_GET['display'] == 'div') {
        $display = 'table';
      }
      else {
        $display = $switch_style; // if the display style is not properly set in the url, choose the oposite of the one enabled in the DB.
      }    
    }
    else {
      $display = $switch_style; // if the display style is not set in the url, choose the oposite of the one enabled in the DB.
    }
    
    $curr_url = $base_url . request_uri();

    if (isset($_GET['page'])) {
      $curr_url = str_replace('&display=' . $_GET['display'], '', $curr_url);
      $curr_url = $curr_url . '&display=' . $display;
    }
    else {
      $curr_url = str_replace('?display=' . $_GET['display'], '', $curr_url);
      $curr_url = $curr_url . '?display=' . $display;
    }

    $variables['switch_rendered'] = theme('islandora_solr_custom_switch', $curr_url);
  
  }
}



/**
 * Theme function for theming the switch link.
 * The theme function is really minimal, but allows for overriding they way the switch is themed.
 *
 * @return rendered switch link
 */
 
function theme_islandora_solr_custom_switch($curr_url) {

  return l(t('Switch display'), $curr_url);

}



/**
 * Theme function for theming the table display.
 *
 * @return rendered table display
 */
 
function theme_islandora_solr_custom_table($db_values, $results) {
  $output = '';
    // Table headers.
    $header = array();
    foreach ($db_values as $key => $result) {
      if ($result['exclude_label'] == 0) {
        $header[] = array(
                      'data' => t($result['label']),
                      //'field' => $result['field'], // I guess sorting is only possible when the result is fetched from the DB.
                    );
      }
      else {
        $header[] = '';
      }
    }
    // Table rows
    $rows = array();
    foreach ($results as $key => $result) {
      $row = array();

      foreach ($result as $key => $value) {
        $row[] = array(
                  'data' => $value['value'],
                 );
      }
      $rows[] = $row;
    }
    
    $output = theme('table', $header, $rows);

  return $output;
}




/**
 * Theme function for theming the RSS secondary display.
 *
 * @return rendered RSS display
 */

function theme_islandora_solr_custom_rss($channel, $item) {

  drupal_set_header('Content-Type: application/rss+xml; charset=utf-8');

  $items = '';
  foreach ($item as $feed) {
    dsm($feed);
    $items .= format_rss_item($feed['title'], $feed['link'], $feed['description'], $feed['items']);
  }
  dsm($items);
  $site_name = variable_get('site_name', 'Drupal');
  $url = url((isset($category) ? 'aggregator/categories/' . $category->cid : 'aggregator'), array('absolute' => TRUE));
  $description = isset($category) ? t('@site_name - aggregated feeds in category @title', array('@site_name' => $site_name, '@title' => $category->title)) : t('@site_name - aggregated feeds', array('@site_name' => $site_name));

  $output = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
  $output .= "<rss version=\"2.0\">\n";
  $output .= format_rss_channel(t('@site_name aggregator', array('@site_name' => $site_name)), $url, $description, $items);
  $output .= "</rss>\n";

  print $output;

  
}
