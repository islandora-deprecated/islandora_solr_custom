<?php

/*
 * contains methods to search solr and display results.  depends on Apache_Solr_Php client.
 */

module_load_include('inc', 'islandora_solr_search', 'IslandoraSolrResults');

/**
 * Extention of IslandoraSolrResults for templating purposes.
 * This overrides the displayResults function to provide an alternate display type.
 */

class IslandoraSolrResultsRSS extends IslandoraSolrResults {

  function IslandoraSolrResultsRSS() {
    module_load_include('php', 'islandora_solr_search', 'SolrPhpClient/Apache/Solr/Service');
  }

  /**
   * Outputs results basically in the normal way, but with thumbnails pulled
   * from the Fedora repository.
   *
   * @param $results
   *   A solr resultset object.
   *
   * @return
   *   html output for the resultset. Note: we currently create this
   *   output manually, should refactor to use drupal forms api.
   */
  function printRSS($solrQueryProcessor, $title = "Search Results") {
     // Note: solrLimit is really stating the number of rows wanted,
    // not the row number of the upper limit document.  That is, if you
    // want results 40-60, you set solrStart=40, solrLimit=20 -- *not*
    // solrStart=40, solrLimit=60.

    global $base_url;
    
    // First off, update outer limits
    $upperLimit = $solrQueryProcessor->solrResult->response->numFound;
    $lowerLimit = 0;
    $increment = 5000;
    // typical failure point for an unbounded query seems to be around 10000-15000,
    // but we must allow for the every-growing output memory structure.

    $row_count = 0;
    $field_counts = array();
    $values = array();

    $solrQueryProcessor->solrStart = $lowerLimit;
    $solrQueryProcessor->solrLimit = $increment;

    $docfile = tmpfile();
    
    // DB variables
    $db_variables = _islandora_solr_custom_db_values($check = FALSE);
    //dsm($db_variables);

    while( $solrQueryProcessor->solrStart < $upperLimit ) {

      // Perform the incremental re-query.
      $solrQueryProcessor->resetResults();
      $solrQueryProcessor->executeQuery();

      // Update incremental limits for the next round.
      $lowerLimit += $increment;
      $solrQueryProcessor->solrStart = $lowerLimit;

      // If there are no results, skip ahead (i.e. exit).
      if(empty($solrQueryProcessor->solrResult)) {
        continue;
      }

      // else ...
      $rawResponse = $solrQueryProcessor->solrResult->getRawResponse();
      $responseData = json_decode($rawResponse,true); // true == associative array, not object
      dsm($responsedata);
      unset($rawResponse); // unknown usefulness
      
      $docs = $responseData['response']['docs'];
      //dsm($docs);


    }

    
    
  // this is where we switch to rss view
  drupal_set_header('Content-Type: application/rss+xml; charset=utf-8');
  
  // set variables
  $result = array();
  $item = array();
  
  // render the rss <item> elements
  foreach ($docs as $key => $doc) {
    // render single search results
    foreach ($doc as $key => $value) {
      // some values are located in a single array. Revert this:
      if (is_array($value) AND count($value == 1 )) {
        $doc[$key] = $value[0];
      }
    }
    
    // all the variables for the <item> elements are set in this theme function.
    $result = theme('islandora_solr_custom_rss_item', $doc);
    
    $item[] = $result;
  }
  

  $items = '';
  foreach ($item as $feed) {
    $items .= format_rss_item($feed['title'], $feed['link'], $feed['description'], $feed['items']);
  }
  
  // query search terms:
  $query = $solrQueryProcessor->solrQuery;
  
  // get the variables for the <channel> element
  $channel = theme('islandora_solr_custom_rss_channel', $query);
  
  // give the results clean variable names.
  $title = $channel['title'];
  $url = $channel['url'];
  $description = $channel['description'];
  $langcode = $channel['langcode'];
  $args = $channel['args'];

  $output = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
  $output .= "<rss version=\"2.0\">\n";
  $output .= format_rss_channel( $title, $url, $description, $items, $langcode, $args);
  $output .= "</rss>\n";

  print $output;
  }

}

