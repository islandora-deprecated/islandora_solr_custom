<?php

/**
 * @file islandora_solr_custom.tpl.php
 * Islandora solr search results template
 *
 * Variables available:
 * - $variables: all array elements of $variables can be used as a variable. e.g. $base_url equals $variables['base_url']
 * - $base_url: The base url of the current website. eg: http://example.com .
 * - $user: The user object.
 *
 * - $style: the style of the display ('div' or 'table'). Set in admin page by default. Overridden by the query value: ?display=foo
 * - $results: the array containing the solr search results
 * - $table_rendered: If the display style is set to 'table', this will contain the rendered table.
 *    For theme overriding, see: theme_islandora_solr_custom_table() 
 * - $switch_rendered: The rendered switch to toggle between display styles
 *    For theme overriding, see: theme_islandora_solr_custom_switch() 
 *
 */
?>

<?php print $switch_rendered; ?>

<?php if ($style == 'div'): ?>

  <ul class="islandora_solr_results">
    <?php foreach ($results as $id => $result): ?>
      <li class="islandora_solr_result">
        <?php $zebra = 'odd'; ?>
        <?php foreach ($result as $field => $values): ?> 
          <?php 
            $value = $values['value'];
            $label = $values['label'];
            $class = $values['class'];
            $exclude_label = $values['exclude_label'];
            $markup = $values['markup'];
          ?>
          <?php if ($value != '' OR $markup == 1): ?>
            <div class="solr-field <?php print $class . ' ' . $zebra ?>">
              <?php $zebra = ($zebra == 'odd'? 'even' : 'odd' ); ?>  
              <?php if ($exclude_label == 0): ?>
                <div class="label">
                  <label><?php print t($label); ?></label>
                </div>
              <?php endif; ?>
              <div class="value"><?php print $value; ?></div>
            </div>
          <?php endif; ?>
          
        <?php endforeach; ?>
      </li>
    <?php endforeach; ?>
  </ul>

<?php elseif ($style == 'table'): ?>

  <?php print $table_rendered; ?>

<?php endif; ?>
