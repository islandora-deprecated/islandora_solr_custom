<?php


/**
 * Function to return admin setting form
 * @return array() containing the admin form
 */
 
function islandora_solr_custom_admin_settings() {

  $form = array();
  
  $form['islandora_solr_custom_fields'] = array(
    '#type' => 'textarea',
    '#title' => t('Fields to output'),
    '#default_value' => variable_get('islandora_solr_custom_fields', 'dc.title'),
    '#description' => t('Fill out the solr fields you want to display in the search results. One field name per line.'),
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
  
  $form['div'] = array(
    '#type' => 'fieldset',
    '#title' => t('Div based markup'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['div']['islandora_solr_custom_output'] = array(
    '#type' => 'markup',
    '#value' => _islandora_solr_custom_markup_render(variable_get('islandora_solr_custom_fields', 'dc.title')),
  );
  
  $form['#submit'][] = 'islandora_solr_custom_settings_form_submit';
  $form['#theme'] = 'system_settings_form';

  dsm(_islandora_solr_custom_solrconfig());

  return ($form);
}


/**
 * Implementation of hook_form_submit()
 * @param array $form
 * @param array $form_state
 * @return null 
 */
 
function islandora_solr_custom_settings_form_submit($form, &$form_state) {


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
