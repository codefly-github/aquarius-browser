/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

import * as React from 'react'

export default class FavoritesIcon extends React.Component {
  render () {
    return (
      <svg xmlns={'https://www.w3.org/2000/svg'} width={'24'} height={'24'}>
        <g transform={'translate(5, 2)'}>
          <path d={'M14.3553 16H1.64467C0.737333 16 0 15.2627 0 14.3553V1.64467C0 0.738 0.737333 0 1.64467 0H14.3553C15.2627 0 16 0.738 16 1.64467V14.3553C16 15.2627 15.2627 16 14.3553 16ZM14.6667 1.64467C14.6667 1.476 14.524 1.33333 14.3553 1.33333H1.64467C1.476 1.33333 1.33333 1.476 1.33333 1.64467V14.3553C1.33333 14.524 1.476 14.6667 1.64467 14.6667H14.3553C14.524 14.6667 14.6667 14.524 14.6667 14.3553V1.64467ZM12.0133 13.3333C12.0087 13.3333 12.004 13.3333 12 13.3333H4C3.632 13.3333 3.33333 13.0347 3.33333 12.6667C3.33333 10.8753 4.58067 9.444 6.40133 8.90667C5.37333 8.33867 4.66667 7.25533 4.66667 6C4.66667 4.162 6.162 2.66667 8 2.66667C9.838 2.66667 11.3333 4.162 11.3333 6C11.3333 7.25467 10.628 8.33667 9.60133 8.90533C11.3713 9.428 12.5987 10.7973 12.664 12.5207C12.6747 12.5673 12.68 12.6167 12.68 12.6667C12.68 13.0347 12.3813 13.3333 12.0133 13.3333ZM8 4C6.89733 4 6 4.89733 6 6C6 7.10267 6.89733 8 8 8C9.10267 8 10 7.10267 10 6C10 4.89733 9.10267 4 8 4ZM8 10C6.35933 10 5.10067 10.8093 4.75933 12H11.2407C10.8993 10.8093 9.64067 10 8 10Z'} fillRule={'evenodd'} />
        </g>
      </svg>
    )
  }
}
