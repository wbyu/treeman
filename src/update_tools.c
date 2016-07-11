#include <R.h>
#include <Rinternals.h>

// Return matrix of 01s for presence absence of kids by node ID
SEXP getKidsMat(SEXP nids_, SEXP tids_, SEXP prids_)
{
  SEXP res;
  int nids = asInteger(nids_);
  int ntips = length(tids_);
  int* tids = INTEGER(tids_);
  int* prids = INTEGER(prids_);
  PROTECT(res=allocMatrix(INTSXP, nids, ntips));
  int n = length(res);
  int i;
  for(i=0;i<n; i++) {
    INTEGER(res)[i] = 0;
  }
  for(i=0;i<ntips; i++) {
    int tid = tids[i] - 1;
    int id = prids[tid] - 1;
    while(id != -2) {
      INTEGER(res)[id + i * nids] = 1;
      id = prids[id] - 1;
    }
  }
  UNPROTECT(1);
  return res;
}

// Return vector of prdsts of every node
// Only returns true numbers if nds vector provided are rootable
SEXP getPrdstVec(SEXP nids_, SEXP prids_, SEXP spns_)
{
  SEXP res;
  int nids = asInteger(nids_);
  double* spns = REAL(spns_);
  int* prids = INTEGER(prids_);
  PROTECT(res=allocVector(REALSXP, nids));
  int n = length(res);
  int i;
  for(i=0;i<n; i++) {
    REAL(res)[i] = 0;
  }
  for(i=0;i<(nids-1); i++) {
    double spn = spns[i];
    int id = prids[i] - 1;
    while(id != -2) {
      spn += spns[id];
      id = prids[id] - 1;
    }
    REAL(res)[i] += spn;
  }
  UNPROTECT(1);
  return res;
}

// Return vector of pds for every node
// Only returns true numbers if nds vector provided are rootable
SEXP getPdVec(SEXP nids_, SEXP prids_, SEXP spns_)
{
  SEXP res;
  int nids = asInteger(nids_);
  double* spns = REAL(spns_);
  int* prids = INTEGER(prids_);
  PROTECT(res=allocVector(REALSXP, nids));
  int n = length(res);
  int i;
  for(i=0;i<n; i++) {
    REAL(res)[i] = 0;
  }
  int k;
  int j;
  int cc;
  int nxt_cc;
  int ids[nids];
  int nxt_ids[nids];
  for(i=0;i<(nids-1); i++) {
    double pd = 0;
    ids[0] = i;
    cc = 1;
    while(cc>0) {
      nxt_cc = 0;
      for(j=0;j<cc;j++) {
        for(k=0;k<(nids-1); k++) {
          if((prids[k]-1) == ids[j]) {
            pd += spns[j];
            nxt_ids[nxt_cc] = k;
            nxt_cc++;
          }
        }
      }
      cc = nxt_cc;
      for(j=0;j<cc;j++) {
        ids[j] = nxt_ids[j];
      }
    }
    REAL(res)[i] += pd;
  }
  UNPROTECT(1);
  return res;
}