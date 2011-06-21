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
      unset($rawResponse); // unknown usefulness
      
      $docs = $responseData['response']['docs'];
      dsm($docs);


    }

    $results = array();
    
    foreach ($docs as $key => $doc) {
      
        //dsm($doc);
        foreach ($doc as $key => $value) {
          if (is_array($value) AND count($value == 1 )) {
            $doc[$key] = $value[0];
          }
        }
      
        $result['title']       = $doc['dc.title'];
        $result['link']        = $base_url . '/fedora/repository/' . htmlspecialchars($doc['PID'], ENT_QUOTES, 'utf-8');
        $result['description'] = $doc['dc.description'];
        $result['items'][]     = array('key' => 'author', 'value' => $doc['dc.creator']);   
        $result['items'][]     = array('key' => 'guid', 'value' => $doc['PID'], 'attributes' => array('isPermaLink' => 'false',));   // <guid isPermaLink="true">http://inessential.com/2002/09/01.php#a2</guid>
        $result['items'][]     = array('key' => 'enclosure', 'value' => '', 'attributes' => array('url' => 'http://upload.wikimedia.org/wikipedia/commons/7/7a/Basketball.png', 'length' => '1234', 'type' => 'image/png')); // enclosure 	Describes a media object that is attached to the item. More. 	<enclosure url="http://live.curry.com/mp3/celebritySCms.mp3" length="1069871" type="audio/mpeg"/>
        $result['items'][]     = array('key' => 'pubDate', 'value' => $doc['fgs.createdDate']);   
        $result['items'][]     = array('key' => 'category', 'value' => $doc['dc.subject']);
        $result['items'][]     = array('key' => 'comments', 'value' => '');
        $result['items'][]     = array('key' => 'source', 'value' => ''); 
     
      
    $item[] = $result;
    }

    $channel = array();

    dsm($item);
    //dsm($rows);
    
    $output = theme('islandora_solr_custom_rss', $channel, $item);
    
    return $output;
  }

}

