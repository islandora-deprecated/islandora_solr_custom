<?php
// $Id $
/**
 * @file islandora_solr_custom.tpl.php
 * Islandora solr search results template
 *
 * Variables available:
 * - $base_url: The base url of the current website. eg: http://example.com .
 * - $user: The user object.
 *
 * - $style: the style of the display ('div' or 'table')
 * - $results: the array containing the solr search results
 * - $table_rendered: if the display style is set to 'table', this will contain the rendered table
 *
 */
?>

<?php print $switch_rendered; ?>

<?php if ($style == 'div'): ?>

  <ul class="islandora_solr_results">
    <?php foreach ($results as $id => $result): ?>
      <li class="islandora_solr_result">
        <?php foreach ($result as $field => $values): ?> 
          <?php 
            $value = $values['value'];
            $label = $values['label'];
            $class = $values['class'];
            $exclude_label = $values['exclude_label'];
            $markup = $values['markup'];
            $zebra = $values['zebra'];
          ?>
          <?php if ($markup == 1): ?>
            <div class="solr-field <?php print $class.' '.$zebra ?>">  
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


<?php
